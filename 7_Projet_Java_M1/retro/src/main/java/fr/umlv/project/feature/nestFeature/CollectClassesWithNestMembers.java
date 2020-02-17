package fr.umlv.project.feature.nestFeature;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Objects;
import java.util.Set;

import org.objectweb.asm.ClassReader;
import org.objectweb.asm.ClassVisitor;
import org.objectweb.asm.FieldVisitor;
import org.objectweb.asm.MethodVisitor;
import org.objectweb.asm.Opcodes;

import fr.umlv.project.feature.FeatureCollector;

public class CollectClassesWithNestMembers implements FeatureCollector {

	private final Map<String, byte[]> classMap;

	/** Name of all the classes with nest attribute and their info */
	private final Map<String, NestClassInfo> nestClassesInfosMap = new HashMap<>();
	
	/** Name of all the classes that need new synthetic method with a Set of classAtribute that contains informations. */
	private final Map<String, Set<ClassAtribute>> classesThatNeedSyntheticMethod = new HashMap<>();

	private final int api;

	/**
	 * Private constructor.
	 * 
	 * @param classMap: Map that associate a className with it's bytes content.
	 */
	private CollectClassesWithNestMembers(int api, Map<String, byte[]> classMap) {
		this.classMap = Collections.unmodifiableMap(classMap);
		this.api = api;
	}

	public static CollectClassesWithNestMembers collectClassesWithNestMembersFactory(int api,
			Map<String, byte[]> classMap) {
		Objects.requireNonNull(classMap);
		CollectClassesWithNestMembers nestCollector = new CollectClassesWithNestMembers(api, classMap);
		nestCollector.collect();
		return nestCollector;
	}

	/**
	 * Method that allow you to collect all the classes that have a nestMembers or
	 * nestHost properties.
	 * 
	 * @return a set of strings of all the class names that implements the
	 *         NestFeature.
	 */
	private void collect() {
		classMap.values().forEach(byteContent -> {
			var cr = new ClassReader(byteContent);
			cr.accept(new CollectNestClasses(api), 0);
		});
		nestClassesInfosMap.keySet().forEach(className -> {
			var cr = new ClassReader(classMap.get(className + ".class"));
			cr.accept(new AnalyzeIfNeedSyntheticMethodAndCollect(api), 0);
		});
	}

	@Override
	public Set<String> getClassesNamesFeature() {
		return new HashSet<>(nestClassesInfosMap.keySet());
	}

	@Override
	public void printFeature() {
		nestClassesInfosMap.forEach((className, nestClassInfo) -> {
			System.out.println(nestClassInfo.toString());
		});
	}
	
	@Override
	public List<String> getFeaturesMessage() {
		List<String> arr = new ArrayList<>();
		nestClassesInfosMap.forEach((className, nestClassInfo) -> 
			arr.add(nestClassInfo.toString())
		);
		return arr;
	}
	
	

	public Map<String, NestClassInfo> getNestClassesInfos() {
		return new HashMap<>(nestClassesInfosMap);
	}

	public Map<String, Set<ClassAtribute>> getClassesThatNeedSyntheticMethod() {
		return new HashMap<>(classesThatNeedSyntheticMethod);
	}

	/**
	 * A ClassVisitor that add all the classes with Nest attribute in the Set of {@code nestClassesSet}, 
	 * and create a {@code NestClassInfo} and extract in it informations 
	 * like if it have any private methods or private fields and
	 * put them in the Map {@code nestClassesInfosMap}.
	 */
	private class CollectNestClasses extends ClassVisitor {
		
		private NestClassInfo ncInfo;

		private boolean isNested = false;

		/** The current name of the class visited. */
		private String classNameOwner;
		
		private CollectNestClasses(int api) {
			super(api);
		}


		@Override
		public void visit(int version, int access, String name, String signature, String superName,
				String[] interfaces) {
			this.classNameOwner = name;
			this.ncInfo = new NestClassInfo(name);
		}

		@Override
		public void visitNestHost(String nestHost) {
			isNested = true;
			ncInfo.addNestHost(nestHost);
			super.visitNestHost(nestHost);
		}

		@Override
		public void visitNestMember(String nestMember) {
			isNested = true;
			ncInfo.addNestMembers(nestMember);
			super.visitNestMember(nestMember);
		}

		@Override
		public FieldVisitor visitField(int access, String name, String descriptor, String signature, Object value) {
			if (isPrivate(access)) {
				ncInfo.addPrivateAttribute(new FieldInfo(name, descriptor));
			}
			return super.visitField(access, name, descriptor, signature, value);
		}

		@Override
		public MethodVisitor visitMethod(int access, String name, String descriptor, String signature,
				String[] exceptions) {
			if (isPrivate(access)) {
				ncInfo.addPrivateAttribute(new MethodInfo(name, descriptor));
			}

			return super.visitMethod(access, name, descriptor, signature, exceptions);
		}

		@Override
		public void visitEnd() {
			if(isNested) {
				nestClassesInfosMap.put(classNameOwner, ncInfo);
			}
			super.visitEnd();
		}

	}
	
	private static boolean isPrivate(int access) {
		return (access & Opcodes.ACC_PRIVATE) == Opcodes.ACC_PRIVATE;
	}

	/** Analyze if the class access to a private field or method from the outer class. */
	private class AnalyzeIfNeedSyntheticMethodAndCollect extends ClassVisitor {

		private String classOwner;

		private AnalyzeIfNeedSyntheticMethodAndCollect(int api) {
			super(api);
		}

		@Override
		public void visit(int version, int access, String name, String signature, String superName,
				String[] interfaces) {
			this.classOwner = name;
			classesThatNeedSyntheticMethod.putIfAbsent(classOwner, new HashSet<>());
		}

		@Override
		public MethodVisitor visitMethod(int access, String name, String descriptor, String signature,
				String[] exceptions) {
			MethodVisitor mv = super.visitMethod(access, name, descriptor, signature, exceptions);
			return new MethodVisitor(api, mv) {

				@Override
				public void visitFieldInsn(int opcode, String owner, String name, String descriptor) {
					if (opcode == Opcodes.GETFIELD || opcode == Opcodes.PUTFIELD) {
						var currField = new FieldInfo(name, descriptor);
						if(checkIfNeedSynthMethodAndAddIt(opcode, owner, currField)) {
							var fieldToAdd = new FieldInfo(generateNameForSynthMethodFactory(opcode, name), descriptor);
							addInClassesThatNeedSyntheticMethod(owner, fieldToAdd);
						}
					}
					super.visitFieldInsn(opcode, owner, name, descriptor);
				}
	
				@Override
				public void visitMethodInsn(int opcode, String owner, String name, String descriptor,
						boolean isInterface) {
					if (opcode == Opcodes.INVOKEVIRTUAL || opcode == Opcodes.INVOKESTATIC) {
						var currMethod = new MethodInfo(name, descriptor);
						if(checkIfNeedSynthMethodAndAddIt(opcode, owner, currMethod)) {
							var methodToAdd = new MethodInfo(generateNameForSynthMethodFactory(opcode, name), descriptor);
							addInClassesThatNeedSyntheticMethod(owner, methodToAdd);
						}

						
					}
					super.visitMethodInsn(opcode, owner, name, descriptor, isInterface);
				}
				
				
				private boolean checkIfNeedSynthMethodAndAddIt(int opcode, String owner, ClassAtribute currFielOrMethod) {
					// If it access to a field that is not in it's own class.
					if (!owner.equals(classOwner)) {
						// But in a nest class and for whom this field or method is private.
						if (nestClassesInfosMap.containsKey(owner)
								&& nestClassesInfosMap.get(owner).contains(currFielOrMethod)) {
							return true;
						}
					}
					return false;
				}
				
				
				/**
				 * Generate the name of the synthetic method that will eventually be created if
				 * backported.
				 * 
				 * @return the name with the format : <b>access$opcode$fieldOrMethodName<b>
				 */
				private String generateNameForSynthMethodFactory(int opcode, String fieldOrMethodName) {
					return "access$" + opcode + "$" + fieldOrMethodName;
				}
	
				private void addInClassesThatNeedSyntheticMethod(String owner, ClassAtribute ca) {
					if (classesThatNeedSyntheticMethod.containsKey(owner)) {
						classesThatNeedSyntheticMethod.get(owner).add(ca);
					} else {
						classesThatNeedSyntheticMethod.put(owner, new HashSet<>(Arrays.asList(ca)));
					}
				}
			};
			
			
			
		}

	}

}
