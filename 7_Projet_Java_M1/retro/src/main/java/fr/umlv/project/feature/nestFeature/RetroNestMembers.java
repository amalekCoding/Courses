package fr.umlv.project.feature.nestFeature;

import java.util.Map;
import java.util.Objects;
import java.util.Set;

import org.objectweb.asm.ClassVisitor;
import org.objectweb.asm.MethodVisitor;
import org.objectweb.asm.Opcodes;
import org.objectweb.asm.Type;

import fr.umlv.project.feature.Util;

public class RetroNestMembers extends ClassVisitor {

	/** All the classes that have Nest attributes with their info. */
	private final Map<String, NestClassInfo> nestClassesInfos;

	/**
	 * All the classes that have inner classes that access to their private fields
	 * or methods.
	 * <p>
	 * Key : name of the outer class.
	 * </p>
	 * Value : Set of private fields/methods that we need to create a synthetic
	 * method for each.
	 */
	private final Map<String, Set<ClassAtribute>> classesThatNeedSyntheticMethod;

	/** The current name of the class that we are visiting. */
	private String classOwner;

	/**
	 * Constructor
	 * 
	 * @param api
	 * @param classVisitor
	 * @param nestClassesInfos
	 * @param classesThatNeedSyntheticMethod
	 */
	public RetroNestMembers(int api, ClassVisitor classVisitor, Map<String, NestClassInfo> nestClassesInfos,
			Map<String, Set<ClassAtribute>> classesThatNeedSyntheticMethod) {
		super(api, classVisitor);
		this.nestClassesInfos = Objects.requireNonNull(nestClassesInfos);
		this.classesThatNeedSyntheticMethod = Objects.requireNonNull(classesThatNeedSyntheticMethod);
	}

	@Override
	public void visit(int version, int access, String name, String signature, String superName, String[] interfaces) {
		this.classOwner = name;
		super.visit(version, access, name, signature, superName, interfaces);
	}

	@Override
	public MethodVisitor visitMethod(int access, String name, String descriptor, String signature,
			String[] exceptions) {
		MethodVisitor mv = super.visitMethod(access, name, descriptor, signature, exceptions);
		return new RemplaceCallToSyntheticAccessMethodIfRequired(api, mv);
	}

	@Override
	public void visitNestMember(String nestMember) {
		// Delete NestMember attribute.
	}

	@Override
	public void visitNestHost(String nestHost) {
		// Delete NestHost attribute.
	}

	@Override
	public void visitEnd() {
		if (classesThatNeedSyntheticMethod.containsKey(classOwner)) {
			classesThatNeedSyntheticMethod.get(classOwner).forEach(methodOrFieldName -> {
				generateSyntheticAccessMethod(methodOrFieldName.getName(), methodOrFieldName.getDesc());
			});
		}

		super.visitEnd();
	}

	/**
	 * @param methodOrFieldName in the format : access$opcode$fieldOrMethodName
	 * @return fieldOrMethodName
	 */
	private String getInternalName(String methodOrFieldName) {
		return methodOrFieldName.substring(methodOrFieldName.lastIndexOf("$") + 1);
	}

	/**
	 * @param name in the format : access$opcode$fieldOrMethodName
	 * @return the value of the opcode
	 */
	private int getOpcodeFromName(String name) {
		return Integer.parseInt(name.substring(name.indexOf("$") + 1, name.lastIndexOf("$")));
	}
			
	private String getDescMethodSynthToGenerate(String name, String descriptor) {
		switch (getOpcodeFromName(name)) {
		case Opcodes.GETFIELD:
			return "(L" + classOwner + ";)" + descriptor;
		case Opcodes.PUTFIELD:
			return "(L" + classOwner + ";" + descriptor + ")V";
		case Opcodes.INVOKEVIRTUAL:
			return descriptor.replace("(", "(L" + classOwner + ";");
		case Opcodes.INVOKEDYNAMIC:
			return descriptor;
		default:
			throw new IllegalStateException();
		}
	}

	private int getOpcodeCallInAccessToPrivateMethOrField(int opcode) {
		if(opcode == Opcodes.GETFIELD || opcode == Opcodes.PUTFIELD || opcode == Opcodes.INVOKEVIRTUAL) {
			return opcode;
		}
		else if(opcode == Opcodes.INVOKEDYNAMIC) {
			return Opcodes.INVOKESTATIC;
		}
		else {
			throw new IllegalStateException();
		}
	}

	private void generateSyntheticAccessMethod(String name, String descriptor) {
		String accessMethDescriptor = getDescMethodSynthToGenerate(name, descriptor);
		Type acessArgs[] = Type.getArgumentTypes(accessMethDescriptor);
		var methodVisitor = cv.visitMethod(Opcodes.ACC_STATIC | Opcodes.ACC_SYNTHETIC, name, accessMethDescriptor, null, null);
		methodVisitor.visitCode();
		generateBodySynthAccessMethod(methodVisitor, name, descriptor, acessArgs);
		methodVisitor.visitInsn(Util.getReturnOpcodesFromType(Type.getReturnType(accessMethDescriptor)));
		methodVisitor.visitMaxs(-1, -1);
		methodVisitor.visitEnd();
	}
	
	
	private void generateBodySynthAccessMethod(MethodVisitor mv, String name, String descriptor, Type acessArgs[]) {
		int opcode = getOpcodeFromName(name);
		int opcodeCallInAccessToPrivateMethOrField = getOpcodeCallInAccessToPrivateMethOrField(opcode);
		for (var i = 0; i < acessArgs.length; ++i) {
			mv.visitVarInsn(Util.getLoadOpcodesFromType(acessArgs[i]), i);
		}
		generateRightInstructionCall(mv, name, descriptor, opcode, opcodeCallInAccessToPrivateMethOrField);
	}
	
	
	private void generateRightInstructionCall(MethodVisitor mv, String name, String descriptor, int opcode, int opcodeCallInAccessToPrivateMethOrField) {
		if (opcode == Opcodes.INVOKEVIRTUAL | opcode == Opcodes.INVOKEDYNAMIC) {
			mv.visitMethodInsn(opcodeCallInAccessToPrivateMethOrField, classOwner, getInternalName(name),
					descriptor, false);
		} else if (opcode == Opcodes.PUTFIELD | opcode == Opcodes.GETFIELD) {
			mv.visitFieldInsn(opcodeCallInAccessToPrivateMethOrField, classOwner, getInternalName(name),
					descriptor);
		}
	}
	

	private class RemplaceCallToSyntheticAccessMethodIfRequired extends MethodVisitor {

		public RemplaceCallToSyntheticAccessMethodIfRequired(int api, MethodVisitor methodVisitor) {
			super(api, methodVisitor);
		}

		@Override
		public void visitFieldInsn(int opcode, String owner, String name, String descriptor) {
			if (opcode == Opcodes.GETFIELD | opcode == Opcodes.PUTFIELD) {
				if (checkIfNeedToReplaceCall(owner, new FieldInfo(name, descriptor))) {
					descriptor = (opcode == Opcodes.GETFIELD) ? "(L" + owner + ";)" + descriptor
							: "(L" + owner + ";" + descriptor + ")V";
					super.visitMethodInsn(Opcodes.INVOKESTATIC, owner, "access$" + opcode + "$" + name, descriptor,
							false);
					return;
				}
			}
			super.visitFieldInsn(opcode, owner, name, descriptor);
		}

		@Override
		public void visitMethodInsn(int opcode, String owner, String name, String descriptor, boolean isInterface) {
			if (opcode == Opcodes.INVOKESTATIC | opcode == Opcodes.INVOKEVIRTUAL) {
				if (checkIfNeedToReplaceCall(owner, new MethodInfo(name, descriptor))) {
					super.visitMethodInsn(Opcodes.INVOKESTATIC, owner, "access$" + opcode + "$" + name,
							descriptor.replace("(", "(L" + owner + ";"), false);
					return;
				}
			}
			super.visitMethodInsn(opcode, owner, name, descriptor, isInterface);
		}

		/** Check if it access to a method or field that is not in it's own class but is present in a other 
		 *  nested class for whom we already created a new synthetic function to allow the access to it. */
		private boolean checkIfNeedToReplaceCall(String owner, ClassAtribute currMethOrField) {
			if (!owner.equals(classOwner)) {
				if (nestClassesInfos.containsKey(owner) && nestClassesInfos.get(owner).contains(currMethOrField)) {
					if (classesThatNeedSyntheticMethod.get(owner).stream().anyMatch(
							methodOrField -> (currMethOrField.getName().equals(getInternalName(methodOrField.getName()))
									&& currMethOrField.getDesc().equals(methodOrField.getDesc())))) {
						return true;
					}
				}
			}
			return false;
		}
	}

}
