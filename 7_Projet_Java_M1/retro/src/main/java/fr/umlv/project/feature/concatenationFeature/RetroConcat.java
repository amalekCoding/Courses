package fr.umlv.project.feature.concatenationFeature;

import java.util.Arrays;
import java.util.List;
import java.util.Map;
import java.util.Objects;

import org.objectweb.asm.ClassVisitor;
import org.objectweb.asm.Handle;
import org.objectweb.asm.MethodVisitor;
import org.objectweb.asm.Opcodes;
import org.objectweb.asm.Type;

import fr.umlv.project.feature.Util;


public class RetroConcat extends ClassVisitor {

	/** Every concatenation info present in the class with the name of the 
	 *   new synthetic method that will be created to be replaced by. */
	private final Map<ConcatInfo, String> concatMapInfo;


	private String classOwnerName;
	
	
	public RetroConcat(int api, ClassVisitor classVisitor, Map<ConcatInfo, String> concatMapInfo) {
		super(api, classVisitor);
		this.concatMapInfo = Objects.requireNonNull(concatMapInfo);
	}
	

	@Override
	public void visit(int version, int access, String name, String signature, String superName, String[] interfaces) {
		super.visit(version, access, name, signature, superName, interfaces);
		this.classOwnerName = name;
		concatMapInfo.forEach((concatInfo, synthName) -> {
				createSynthMethod(synthName, concatInfo.getConcatDesc(), concatInfo.getNewPaternWanted(), concatInfo.getNewDelimiter());
		});
	}
	
	@Override
	public MethodVisitor visitMethod(int access, String name, String desc, String signature, String[] exceptions) {
		MethodVisitor mv = super.visitMethod(access, name, desc, signature, exceptions);
		return new RemplaceInvokDynCallToSynthMethodGenerated(api, mv, name, desc);
	}
	


	private void createSynthMethod(String synthMethodName, String desc, String patternString, String delimiter) {
		List<String> patternArr = Arrays.asList(patternString.split("((?<="+delimiter+")|(?="+delimiter+"))"));

		MethodVisitor mv = cv.visitMethod(Opcodes.ACC_SYNTHETIC | Opcodes.ACC_PRIVATE | Opcodes.ACC_STATIC, synthMethodName, desc, null, null);
		mv.visitCode();
		initializeStringBuilder(mv);
		appendInTheStringBuild(mv, desc, patternArr, delimiter);
		endStringBuilder(mv);
		endMvSimple(mv, Opcodes.ARETURN);
	}
	
	private void endMvSimple(MethodVisitor mv, int opcode) {
		mv.visitInsn(Opcodes.ARETURN);
		mv.visitMaxs(-1, -1);
		mv.visitEnd();
	}
	
	
	private void initializeStringBuilder(MethodVisitor mv) {
		mv.visitTypeInsn(Opcodes.NEW, "java/lang/StringBuilder");
		mv.visitInsn(Opcodes.DUP);
		mv.visitMethodInsn(Opcodes.INVOKESPECIAL, "java/lang/StringBuilder", "<init>", "()V", false);
	}
	
	private void endStringBuilder(MethodVisitor mv) {
		mv.visitMethodInsn(Opcodes.INVOKEVIRTUAL, "java/lang/StringBuilder", "toString", "()Ljava/lang/String;", false);
	}

	
	
	private void appendInTheStringBuild(MethodVisitor mv, String desc, List<String> patternArr, String delimiter) {				
		Type[] args = Type.getArgumentTypes(desc);
		for (int i = 0, indiceArg = 0, stackSize = 0 ; i < patternArr.size() ; ++i) {
			// If is an argument captured dynamically 
			if (patternArr.get(i).equals(delimiter)) {
				appendArgCatchByInvokDynm(mv, args[indiceArg], stackSize);
				stackSize += checkIfTypeIsLongOrDouble(args[indiceArg].toString()) ? 2 : 1;
				indiceArg++;
			}
			else {
				appendConstantArgInTheStringBuilder(mv, patternArr.get(i));
			}
		}
	}
	
	private boolean checkIfTypeIsLongOrDouble(String str) {
		return (str.equals("J") || str.equals("D"));
	}
	
	
	private void appendConstantArgInTheStringBuilder(MethodVisitor mv, String arg) {
		mv.visitLdcInsn(arg);
		appendArgInTheStringBuilder(mv, "Ljava/lang/String;");
	}
	
	
	private void appendArgCatchByInvokDynm(MethodVisitor mv, Type argType, int stackSize) {
		mv.visitVarInsn(Util.getLoadOpcodesFromType(argType), stackSize);
		String arg = argType.toString();
		if (Util.getLstPrimaryType().contains(arg) || arg.contains("String")) {
			appendArgInTheStringBuilder(mv, arg);
		} 
		else {
			appendArgInTheStringBuilder(mv, "Ljava/lang/Object;");
		}
	}
	
	private void appendArgInTheStringBuilder(MethodVisitor mv, String desc) {
		mv.visitMethodInsn(Opcodes.INVOKEVIRTUAL, "java/lang/StringBuilder", "append",
				"(" + desc + ")Ljava/lang/StringBuilder;", false);
	}
	


	private class RemplaceInvokDynCallToSynthMethodGenerated extends MethodVisitor {

		private String methodName;
		private String methodDesc;

		public RemplaceInvokDynCallToSynthMethodGenerated(int api, MethodVisitor methodVisitor, String name,
				String desc) {
			super(api, methodVisitor);
			methodName = name;
			methodDesc = desc;
		}

		@Override
		public void visitInvokeDynamicInsn(String name, String descriptor, Handle bootstrapMethodHandle, Object... bootstrapMethodArguments) {
			if (bootstrapMethodHandle.getOwner().equals("java/lang/invoke/StringConcatFactory") && name.equals("makeConcatWithConstants")) {
				ConcatInfo ci = new ConcatInfo(classOwnerName, methodName, methodDesc, descriptor, "", "", 0);
				if (concatMapInfo.containsKey(ci)) {
					mv.visitMethodInsn(Opcodes.INVOKESTATIC, classOwnerName, concatMapInfo.get(ci), descriptor, false);
					return;
				}
				throw new IllegalStateException("Concatenation detected not known !");
			}
			mv.visitInvokeDynamicInsn(name, descriptor, bootstrapMethodHandle, bootstrapMethodArguments);
		}
	}

}