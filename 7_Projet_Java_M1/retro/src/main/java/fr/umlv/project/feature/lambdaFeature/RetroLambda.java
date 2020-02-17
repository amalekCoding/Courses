package fr.umlv.project.feature.lambdaFeature;

import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

import org.objectweb.asm.ClassVisitor;
import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.FieldVisitor;
import org.objectweb.asm.Handle;
import org.objectweb.asm.MethodVisitor;
import org.objectweb.asm.Opcodes;
import org.objectweb.asm.Type;

import fr.umlv.project.feature.EnumFeature;
import fr.umlv.project.feature.Util;
import fr.umlv.project.parser.WriteUtil;

public class RetroLambda extends ClassVisitor {

	private final int versionTarget;

	private String classNameOwner;

	private int lambdaCounter;

	
	public RetroLambda(int api, ClassVisitor classVisitor, int versionTarget) {
		super(api, classVisitor);
		this.versionTarget = versionTarget;
	}

	
	@Override
	public void visit(int version, int access, String name, String signature, String superName, String[] interfaces) {
		classNameOwner = name;
		super.visit(version, access, name, signature, superName, interfaces);
	}

	@Override
	public MethodVisitor visitMethod(int access, String nameMethod, String descriptor, String signature, String[] exceptions) {
		// If it's a lambda method we make it non-private, so we can call it from the class that we will create.
		if(nameMethod.contains("lambda$")) {
            access &= ~ Opcodes.ACC_PRIVATE;
		}
		MethodVisitor mv = cv.visitMethod(access, nameMethod, descriptor, signature, exceptions);
		return new LambdaReplacer(api, mv, nameMethod, classNameOwner, descriptor); 
	}
	
	

	private class LambdaReplacer extends MethodVisitor {

	    private final String className;
	    private final String nameMethod;
	    private final String descMethod;
	    private ClassWriter currentNewCw;
	    private final Map<String, String> mapIntrfcFuncCalled = new HashMap<>();

	    private String newClassNameForLambda;
		
	    public LambdaReplacer(int api, MethodVisitor methodVisitor, String nameMethod, String className, String descMethod) {
			super(api, methodVisitor);
			this.className = className;
			this.nameMethod = nameMethod;
			this.descMethod = descMethod;
			newClassNameForLambda = className+"$$Lambda$" + lambdaCounter;
		}

	    
	    @Override
	    public void visitInvokeDynamicInsn(String methodName, String descriptor, Handle bootstrapMethodHandle, Object... bootstrapMethodArguments) {
			if(bootstrapMethodHandle.getOwner().equals("java/lang/invoke/LambdaMetafactory")) {
				replaceLambda(methodName, descriptor, bootstrapMethodHandle, bootstrapMethodArguments);
				return ;
			}
			super.visitInvokeDynamicInsn(methodName, descriptor, bootstrapMethodHandle, bootstrapMethodArguments);
		}
	    
	    
	    private void replaceLambda(String methodName, String descriptor, Handle bootstrapMethodHandle, Object... bootstrapMethodArguments) {
	    	String functInterfaceCalled = Type.getReturnType(descriptor).getClassName().replace(".", "/");
			replaceInvokDynCallToNewMethodFactory(methodName, descriptor, functInterfaceCalled);
			replaceLambdaByCreatingClass(versionTarget, methodName, descriptor, functInterfaceCalled, bootstrapMethodArguments);
			if(canAddNestAtribute(versionTarget)) {
				cv.visitNestMember(newClassNameForLambda);
			}
			lambdaCounter++;
	    }
		
	    @Override
	    public void visitMethodInsn(int opcode, String owner, String name, String descriptor, boolean isInterface) {
	    	if(opcode == Opcodes.INVOKEINTERFACE && mapIntrfcFuncCalled.containsKey(owner)) {
		    	super.visitMethodInsn(opcode, getInterfaceCreatedName(), name, descriptor, isInterface);
		    	return;
	    	}
	    	super.visitMethodInsn(opcode, owner, name, descriptor, isInterface);
	    	
	    }
	    
	    
	    private void replaceInvokDynCallToNewMethodFactory(String methodName, String descriptor, String intrFuncCalled) {
	    	mapIntrfcFuncCalled.put(intrFuncCalled, getInterfaceCreatedName());
	    	String newDescriptor = descriptor.substring(0, descriptor.indexOf(")")+1) + "L"+getInterfaceCreatedName()+";";
		    super.visitMethodInsn(Opcodes.INVOKESTATIC, newClassNameForLambda, getFactoryMethodName(), newDescriptor, false);
	    }
	  
	    
		private String getFactoryMethodName() {
			return "lambdaFactory$";
		}
		
	    
		/** Create a new class which is an inner class of the current class visited. */
		private void replaceLambdaByCreatingClass(int versionTarget, String name, String descriptor, String functInterfaceCalledName, Object... bootstrapMethodArguments) {
			var lambdaFilesPath = "mybinLambda/";
			this.currentNewCw = WriteUtil.createClassWriterAndNewFile(lambdaFilesPath+newClassNameForLambda);
			currentNewCw.visit(versionTarget, Opcodes.ACC_SYNTHETIC | Opcodes.ACC_FINAL | Opcodes.ACC_PROTECTED, newClassNameForLambda, null, "java/lang/Object", new String[]{getInterfaceCreatedName()} );
			
			generateBodyOfNewClassCreated(name, descriptor, bootstrapMethodArguments);
			createInterfaceClass(versionTarget, name, descriptor, lambdaFilesPath, bootstrapMethodArguments);
			addAtribute(versionTarget);
			writeClassCreated(currentNewCw, lambdaFilesPath+newClassNameForLambda);
		}
		
		private void addAtribute(int versionTarget) {
			if(canAddNestAtribute(versionTarget)) {
				currentNewCw.visitNestHost(classNameOwner);
			}
			currentNewCw.visitInnerClass(newClassNameForLambda, null, null, 0);
			currentNewCw.visitOuterClass(className, nameMethod, descMethod);
		}
		
		private void writeClassCreated(ClassWriter cw, String name) {
			try {
				WriteUtil.writeByteToFile(name, cw.toByteArray());
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		
		private String getInterfaceCreatedName() {
			return "interface$"+newClassNameForLambda;
		}
		
		private void createInterfaceClass(int versionTarget, String name, String descriptor, String lambdaFilesPath, Object... bootstrapMethodArguments) {
			String newInterfFuncDesc = bootstrapMethodArguments[0].toString().substring(bootstrapMethodArguments[0].toString().indexOf("("));
			var cw = WriteUtil.createClassWriterAndNewFile(lambdaFilesPath+getInterfaceCreatedName());
			cw.visit(versionTarget, Opcodes.ACC_PUBLIC|Opcodes.ACC_STATIC|Opcodes.ACC_INTERFACE|Opcodes.ACC_ABSTRACT, getInterfaceCreatedName(), null, "java/lang/Object", null );
						cw.visitMethod(Opcodes.ACC_PUBLIC + Opcodes.ACC_ABSTRACT, name, newInterfFuncDesc, null, null);
			writeClassCreated(cw, lambdaFilesPath+getInterfaceCreatedName());
		}
		
		
		private boolean canAddNestAtribute(int targetVersion) {
			return !EnumFeature.requireBackPort(EnumFeature.Nest, targetVersion);
		}
		
		

		
		
		private void generateBodyOfNewClassCreated(String name, String descriptor, Object... bootstrapMethodArguments) {
			var interfaceDesc = descriptor.substring(0, descriptor.lastIndexOf(")") + 1);
			// If the lambda doesn't capture from the outer class local variable or instance field.
			if(interfaceDesc.equals("()")) {
				generateBodyClassForSimpleStaticAccess(descriptor);
			}
			else {
				generateConstructorAndFields(currentNewCw, interfaceDesc);
				generateFactoryMethod(currentNewCw, descriptor) ;
			}
			addBridgeMethod(name, descriptor, bootstrapMethodArguments);
		}
		
		
		
		private void generateBodyClassForSimpleStaticAccess(String descriptor) {
			String singletonFieldName = "instance";
			String singletonFieldDesc =  "L"+newClassNameForLambda+";";
			intializeAndCreateSingleton(currentNewCw, singletonFieldName, singletonFieldDesc);
			generateFactoryMethodForStaticAccess(currentNewCw, getFactoryMethodName(), descriptor, singletonFieldName, singletonFieldDesc) ;
		}
		
		
	    /**
	     *  Call this if the lambda don't need a arguments to take. So we use the Singleton Pattern.
		 * @param cv
		 * @param singletonFieldName
		 * @param singletonFieldDesc
		 */
	    private void intializeAndCreateSingleton(ClassVisitor cv, String singletonFieldName, String singletonFieldDesc) {
	    	createSimpleConstructor(cv);

	    	FieldVisitor fv = cv.visitField(Opcodes.ACC_PRIVATE | Opcodes.ACC_STATIC | Opcodes.ACC_FINAL,
	    			singletonFieldName, singletonFieldDesc, null, null);
	        fv.visitEnd();

	        intializeStaticField(cv, singletonFieldName, singletonFieldDesc);
	    }
	    
	    private void endMvSimple(MethodVisitor mv, int opcode) {
	        mv.visitInsn(opcode);
	        mv.visitMaxs(-1, -1);
	        mv.visitEnd();
	    }
	    
	    
	    /** Create a simple constructor with no parameters. */
	    private void createSimpleConstructor(ClassVisitor cv) {
	        MethodVisitor constructor = currentNewCw.visitMethod(Opcodes.ACC_PRIVATE,"<init>","()V", null, null);
	        constructor.visitCode();                            	// Start the code
	        constructor.visitVarInsn(Opcodes.ALOAD, 0);             // Load "this" onto the stack
	        constructor.visitMethodInsn(Opcodes.INVOKESPECIAL,      // Initialize an instance
	                "java/lang/Object", "<init>", "()V", false);                             
	        endMvSimple(constructor, Opcodes.RETURN);
	    }
	    
	    
	    private void intializeStaticField(ClassVisitor cv, String singletonFieldName, String singletonFieldDesc) {
	        MethodVisitor mv = cv.visitMethod(Opcodes.ACC_STATIC, "<clinit>", "()V", null, null);
	        mv.visitCode();
	        mv.visitTypeInsn(Opcodes.NEW, newClassNameForLambda);
	        mv.visitInsn(Opcodes.DUP);
	        mv.visitMethodInsn(Opcodes.INVOKESPECIAL, newClassNameForLambda, "<init>", "()V", false);
	        mv.visitFieldInsn(Opcodes.PUTSTATIC, newClassNameForLambda, singletonFieldName, singletonFieldDesc);
	        endMvSimple(mv, Opcodes.RETURN);
	    }

	    
	    private void generateFactoryMethodForStaticAccess(ClassVisitor cv, String methodName, String interfaceFuncDesc, String singletonFieldName, String singletonFieldDesc) {
	    	String s = interfaceFuncDesc.substring(0, interfaceFuncDesc.indexOf(")")+1) + "L"+getInterfaceCreatedName()+";";
	    	MethodVisitor mv = cv.visitMethod(Opcodes.ACC_PUBLIC | Opcodes.ACC_STATIC, methodName, s, null, null);
	        mv.visitCode();
	        mv.visitFieldInsn(Opcodes.GETSTATIC, newClassNameForLambda, singletonFieldName, singletonFieldDesc);
	        endMvSimple(mv, Opcodes.ARETURN);
	    }
	    


	    private void generateConstructorAndFields(ClassVisitor cv, String desc) {        
	    	Type[] argDesc = Type.getArgumentTypes(desc);
	    	for(int i = 0 ;  i <  argDesc.length ; ++i) {
	    		FieldVisitor fv = cv.visitField(Opcodes.ACC_PRIVATE | Opcodes.ACC_FINAL,
	        			"arg"+(i+1), argDesc[i].getDescriptor(), null, null);
	        	fv.visitEnd();
	        }
	    	generateConstructorWithFieldToFill(desc, argDesc);
	    }
	    
	    
	    private void generateConstructorWithFieldToFill(String desc, Type[] argDesc) {
	    	MethodVisitor con = currentNewCw.visitMethod(Opcodes.ACC_PRIVATE,"<init>", desc+"V", null, null);
	        con.visitCode();                            		// Start the code for this method
	        con.visitVarInsn(Opcodes.ALOAD, 0);                 // Load "this" onto the stack
	        con.visitMethodInsn(Opcodes.INVOKESPECIAL, "java/lang/Object", "<init>", "()V", false);
	        con.visitVarInsn(Opcodes.ALOAD, 0);                 // Load "this" onto the stack      
	        for(int i = 0 ;  i <  argDesc.length ; ++i) {
	        	con.visitVarInsn(Util.getLoadOpcodesFromType(argDesc[i]), i+1); 
	            con.visitFieldInsn(Opcodes.PUTFIELD, newClassNameForLambda, "arg"+(i+1), argDesc[i].getDescriptor() );
	        }
	        endMvSimple(con, Opcodes.RETURN);
	    }
	    
	    
	    
	    
	    private void generateFactoryMethod(ClassVisitor cv, String interfaceFuncDesc) {
	    	String s = interfaceFuncDesc.substring(0, interfaceFuncDesc.indexOf(")")+1) + "L"+getInterfaceCreatedName()+";";
	        MethodVisitor mv = cv.visitMethod(Opcodes.ACC_PUBLIC | Opcodes.ACC_STATIC, getFactoryMethodName(), s, null, null);
	        mv.visitCode();
	        mv.visitTypeInsn(Opcodes.NEW, newClassNameForLambda);
	        mv.visitInsn(Opcodes.DUP);
	        for(int i = 0 ;  i <  Type.getArgumentTypes(interfaceFuncDesc).length ; ++i) {
	        	mv.visitVarInsn(Util.getLoadOpcodesFromType(Type.getArgumentTypes(interfaceFuncDesc)[i]), i);
	        }
	        mv.visitMethodInsn(Opcodes.INVOKESPECIAL, newClassNameForLambda, "<init>", interfaceFuncDesc.substring(0, interfaceFuncDesc.lastIndexOf(")") + 1)+"V", false); 
	        endMvSimple(mv, Opcodes.ARETURN);
	    }
	    

		private void addBridgeMethod(String name, String descriptor, Object... bootstrapMethodArguments) {
			var currBridgeSigna =  bootstrapMethodArguments[2].toString();
			var currBridgeArgTypeArr = Type.getArgumentTypes(currBridgeSigna);
			var returnExpected = Type.getReturnType(currBridgeSigna).toString();
			var bridgeArgs = bootstrapMethodArguments[0].toString();
			bridgeArgs = bridgeArgs.substring(0, bridgeArgs.indexOf(")")+1) + returnExpected;
			var newHandle = (Handle) bootstrapMethodArguments[1];
			var funcMethodCalledBylambdaDesc = parseDescriptorToClassType(newHandle.getDesc());
			var argsInLambdaFromOuter = Type.getArgumentTypes(descriptor);
			
			var newMethodVisitor = currentNewCw.visitMethod(Opcodes.ACC_PUBLIC, name, bootstrapMethodArguments[0].toString(), null, null);
			newMethodVisitor.visitCode();
			generateBodyBridgeMethod(newMethodVisitor, argsInLambdaFromOuter, funcMethodCalledBylambdaDesc, newHandle, bridgeArgs, currBridgeSigna, currBridgeArgTypeArr);
	        endMvSimple(newMethodVisitor, Util.getReturnOpcodesFromType(Type.getReturnType(bootstrapMethodArguments[0].toString())));
		}
		
		
		private void generateBodyBridgeMethod(MethodVisitor newMethodVisitor, Type[] argsInLambdaFromOuter, String[] funcMethodCalledBylambdaDesc, Handle newHandle, String bridgeArgs, String currBridgeSigna, Type[] currBridgeArgTypeArr) {
			if(argsInLambdaFromOuter.length > 0) {
				loadArgFromOuterWithGetField(argsInLambdaFromOuter, currBridgeArgTypeArr, funcMethodCalledBylambdaDesc, newMethodVisitor);
			}
			int nbArgsToAdd = funcMethodCalledBylambdaDesc.length - argsInLambdaFromOuter.length; 
			if(nbArgsToAdd > 0) {
				loadFromTheParameters(newMethodVisitor, nbArgsToAdd, argsInLambdaFromOuter.length, bridgeArgs, funcMethodCalledBylambdaDesc, currBridgeSigna, newHandle);
			}
			callToTheRightFunct(newMethodVisitor, newHandle.getOwner(), newHandle.getName(), newHandle.getDesc());
			convertPrimitiveTypeToObjectTypeIfNeeded(newMethodVisitor, Type.getReturnType(currBridgeSigna), Type.getReturnType(newHandle.getDesc()));
		}
		
		
		private void convertPrimitiveTypeToObjectTypeIfNeeded(MethodVisitor newMethodVisitor, Type objectType, Type primitiveType) {
			if (!objectType.equals(primitiveType)) {
				if(objectType.toString().contains("java/lang")) {
					newMethodVisitor.visitMethodInsn(Opcodes.INVOKESTATIC, objectType.getInternalName(), "valueOf", "("+primitiveType+")"+objectType, false);
				}
			}
		}
			
			
		
		private void loadFromTheParameters(MethodVisitor newMethodVisitor, int nbArgsToAdd, int getFieldCount, String bridgeArgs, String[] funcMethodCalledBylambdaDesc, String bsmArg2, Handle bsmArg1) {
			var bsmArg2ArgTypeArr = Type.getArgumentTypes(bsmArg2);
			var bsmArg1ArgTypeArr = Type.getArgumentTypes(bsmArg1.getDesc());
			for (var i = 0; i < nbArgsToAdd; ++i) {
				newMethodVisitor.visitVarInsn(Util.getLoadOpcodesFromType(bsmArg2ArgTypeArr[i]), i+1); // Load i+1 value onto stack (+1 because at 0 load this)
				if (!bsmArg2.equals(bridgeArgs)) {
					checkCastAndCastParamIfNeeded(newMethodVisitor, bsmArg2, bsmArg2ArgTypeArr, bsmArg1ArgTypeArr, funcMethodCalledBylambdaDesc, i, getFieldCount);				
				}
				else if(!bsmArg2ArgTypeArr[i].equals(bsmArg1ArgTypeArr[i+getFieldCount])) {
					UtilLambda.castPrimaryTypeToPrimary(bsmArg2ArgTypeArr[i], funcMethodCalledBylambdaDesc[i+getFieldCount], newMethodVisitor);
				}
			}
		}
		
		private void checkCastAndCastParamIfNeeded(MethodVisitor newMethodVisitor, String bsmArg2, Type[] bsmArg2ArgTypeArr, Type[] bsmArg1ArgTypeArr, String[] funcMethodCalledBylambdaDesc, int i, int getFieldCount) {
			var argsType = parseDescriptorToClassType(bsmArg2); 
			newMethodVisitor.visitTypeInsn(Opcodes.CHECKCAST, argsType[i].replace(".", "/")); // Check cast
			if (!bsmArg2ArgTypeArr[i].equals(bsmArg1ArgTypeArr[i+getFieldCount])) {
				if(bsmArg2ArgTypeArr[i].getClassName().contains("java.lang.")) {
					boxObjectToPrimaryType(newMethodVisitor, bsmArg2ArgTypeArr[i].getClassName().replace(".", "/"), bsmArg1ArgTypeArr[i+getFieldCount].getClassName(), bsmArg1ArgTypeArr[i+getFieldCount]);
				}
				else {
					UtilLambda.castPrimaryTypeToPrimary(bsmArg2ArgTypeArr[i], funcMethodCalledBylambdaDesc[i+getFieldCount], newMethodVisitor);
				}
			}
		}
		
		
		private void boxObjectToPrimaryType(MethodVisitor newMethodVisitor, String classNameObject, String classNamePrimary, Type primaryType) {
			newMethodVisitor.visitMethodInsn(Opcodes.INVOKEVIRTUAL, classNameObject, classNamePrimary+"Value", "()"+primaryType, false);
		}
		

	    private void loadArgFromOuterWithGetField(Type[] argsInLambdaFromOuter, Type[] currBridgeArgTypeArr, String[] funcMethodCalledBylambdaDesc, MethodVisitor newMethodVisitor) {
				for(int getFieldCount = 0 ;  getFieldCount <  argsInLambdaFromOuter.length; ++getFieldCount) {
					newMethodVisitor.visitVarInsn(Opcodes.ALOAD, 0);                 // Load "this" onto the stack
					newMethodVisitor.visitFieldInsn(Opcodes.GETFIELD, newClassNameForLambda, "arg"+(getFieldCount+1), argsInLambdaFromOuter[getFieldCount].getDescriptor() );
					if(!funcMethodCalledBylambdaDesc[getFieldCount].equals(argsInLambdaFromOuter[getFieldCount].getDescriptor())) {
						if(argsInLambdaFromOuter[getFieldCount].getClassName().contains("java.lang.") && !funcMethodCalledBylambdaDesc[getFieldCount].contains("java.lang.")) {
							boxObjectToPrimaryType(newMethodVisitor, argsInLambdaFromOuter[getFieldCount].getClassName().replace(".", "/"), funcMethodCalledBylambdaDesc[getFieldCount], Type.getObjectType(funcMethodCalledBylambdaDesc[getFieldCount]));
						} else {
							UtilLambda.castPrimaryTypeToPrimary(argsInLambdaFromOuter[getFieldCount], funcMethodCalledBylambdaDesc[getFieldCount], newMethodVisitor);
						}
					}
				}
	    }
	    
		  
	    private void callToTheRightFunct(MethodVisitor mv, String classOwner, String methodName, String descriptor) {
	    	if(methodName.contains("lambda$")) {
	    		classOwner = className;
	    	}
	        mv.visitMethodInsn(Opcodes.INVOKESTATIC, classOwner, methodName, descriptor, false);
	    }
	    
		
		/** If desc = "(ZILjava/lang/String;Ljava/lang/String;)I" renvoie [boolean, int, java.lang.String, java.lang.String]
		**  TODO : Si c'est primaire remplacer par les bon truc.
		**/
		private String[] parseDescriptorToClassType(String desc) {
	        Type[] parametersTypes = Type.getArgumentTypes(desc);
	        String[] parameters2 = new String[parametersTypes.length];

			for(int i = 0 ; i < parametersTypes.length; i++) {
				parameters2[i] = parametersTypes[i].getClassName();
			}
			
			return parameters2;
		}
	}
}
