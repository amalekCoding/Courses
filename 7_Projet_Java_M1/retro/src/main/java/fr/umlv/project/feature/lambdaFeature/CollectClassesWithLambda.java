package fr.umlv.project.feature.lambdaFeature;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Objects;
import java.util.Set;

import org.objectweb.asm.ClassReader;
import org.objectweb.asm.ClassVisitor;
import org.objectweb.asm.Handle;
import org.objectweb.asm.Label;
import org.objectweb.asm.MethodVisitor;
import org.objectweb.asm.Type;

import fr.umlv.project.feature.FeatureCollector;

public class CollectClassesWithLambda implements FeatureCollector {

	/** All the classes bytes content. */
	private final Collection<byte[]> bytesContents;
	
	/** All the classes that contains a lambda feature with all the informations of each lambda. */
	private final HashMap<String, Set<LambdaInfo>> classesWithLambda = new HashMap<>();

	private final int api;
	
	/** Private constructor */
	private CollectClassesWithLambda(int api, Collection<byte[]> bytesContents) {
		this.bytesContents = Collections.unmodifiableCollection(bytesContents);
		this.api = api;
	}

	public static CollectClassesWithLambda collectLambdaFactory(int api, Collection<byte[]> bytesContents) {
		Objects.requireNonNull(bytesContents);
		var lambdasCollector = new CollectClassesWithLambda(api, bytesContents);
		lambdasCollector.collect();
		return lambdasCollector;
	}

	private void collect() {
		bytesContents.forEach(bytesContent -> {
			ClassReader cr = new ClassReader(bytesContent);
			cr.accept(new DetectLambda(api), 0);
		});
	}

	@Override
	public Set<String> getClassesNamesFeature() {
		return new HashSet<>(classesWithLambda.keySet());
	}

	@Override
	public void printFeature() {
		classesWithLambda.forEach((className, lambdasInfos) -> {
			lambdasInfos.forEach(lambdaInfo -> System.out.println(lambdaInfo));
		});
	}
	
	@Override
	public List<String> getFeaturesMessage() {
		List<String> lst = new ArrayList<>();
		classesWithLambda.forEach((className, lambdasInfos) -> {
			lambdasInfos.forEach(lambdaInfo -> lst.add(lambdaInfo.toString()));
		});
		return lst;
	}

	
	private class DetectLambda extends ClassVisitor {

		private DetectLambda(int api) {
			super(api);
		}

	    /** The name of the visited class. */
		private String classOwner;
	    
		/** The name of the currently visited method. */
		private String methodName;
		
		/** The descriptor of the currently visited method. */
		private String methodDesc;
		
		

		@Override
		public void visit(int version, int access, String name, String signature, String superName,
				String[] interfaces) {
			this.classOwner = name;
		}
		

		@Override
		public MethodVisitor visitMethod(int access, String name, String desc, String signature, String[] exceptions) {
			methodName = name;
			methodDesc = desc;
			MethodVisitor mv = super.visitMethod(access, name, desc, signature, exceptions);
			return new FindInvokeDynamicLambdaAndAddIt(api, mv);	
		}
		
		
		/** A MethodVisitor who can allow you to find a Lambda InvokeDynamic call and keep the information 
		 * 	of it into {@code lambdasClasses} hashMap. */
		private class FindInvokeDynamicLambdaAndAddIt extends MethodVisitor {
			
			public FindInvokeDynamicLambdaAndAddIt(int api, MethodVisitor methodVisitor) {
				super(api, methodVisitor);
			}
			
			/** Line of the lambda. */
			private int line;
			
			@Override
			public void visitLineNumber(int line, Label start) {
				this.line = line;
			}
			
			@Override
			public void visitInvokeDynamicInsn(String name, String descriptor, Handle bootstrapMethodHandle,
					Object... bootstrapMethodArguments) {
				if (bootstrapMethodHandle.getOwner().equals("java/lang/invoke/LambdaMetafactory")) {
					String interfaceCalled = Type.getReturnType(descriptor).getInternalName();
					String interfaceDesc = descriptor.substring(0, descriptor.lastIndexOf(")") + 1);
					String lambdaFunctionCalled = bootstrapMethodArguments[1].toString();
					
					addLambdaInfo(interfaceCalled, interfaceDesc, lambdaFunctionCalled, line);
				}
			}
			
			
			/** Add the informations of the new lambda found in the class into the {@code lambdasClasses} hashMap. */
			private void addLambdaInfo(String interfaceCalled, String interfaceDesc, String lambdaFunctionCalled, int line) {
				LambdaInfo lambdaInfo = new LambdaInfo(classOwner.replace("/", "."), methodName, methodDesc,
						interfaceCalled, interfaceDesc, lambdaFunctionCalled, line);
				
				if (classesWithLambda.containsKey(classOwner)) {
					classesWithLambda.get(classOwner).add(lambdaInfo);
				} else {
					classesWithLambda.put(classOwner, new HashSet<>(Arrays.asList(lambdaInfo)));
				}
			}
			
		};
		
		
		
	}

}