package fr.umlv.project.feature.lambdaFeature;

import java.util.Objects;

import org.objectweb.asm.MethodVisitor;
import org.objectweb.asm.Opcodes;
import org.objectweb.asm.Type;

public class UtilLambda {

	public static void castPrimaryTypeToPrimary(Type fromType, String toType, MethodVisitor mv) {
		Objects.requireNonNull(fromType);
		Objects.requireNonNull(mv);
		if (fromType.getClassName() != Objects.requireNonNull(toType)) {
			switch (fromType.getClassName()) {
			case "int":    castFromInt(toType, mv);
				return;
			case "double": castFromDouble(toType, mv);
				return;
			case "float":  castFromFloat(toType, mv);
				return;
			case "long":   castFromLong(toType, mv);
				return;
			}
		}

	}

	private static void castFromInt(String to, MethodVisitor mv) {
		switch (to) {
		case "double": mv.visitInsn(Opcodes.I2D);
			return;
		case "float":  mv.visitInsn(Opcodes.I2F);
			return;
		case "short":  mv.visitInsn(Opcodes.I2S);
			return;
		case "long":   mv.visitInsn(Opcodes.I2L);
			return;
		case "char":   mv.visitInsn(Opcodes.I2C);
			return;
		case "byte":   mv.visitInsn(Opcodes.I2B);
			return;
		}
	}

	private static void castFromDouble(String to, MethodVisitor mv) {
		switch (to) {
		case "float":
			mv.visitInsn(Opcodes.D2F);
			return;
		case "int":
			mv.visitInsn(Opcodes.D2I);
			return;
		case "long":
			mv.visitInsn(Opcodes.D2L);
			return;
		}
	}

	private static void castFromFloat(String to, MethodVisitor mv) {
		switch (to) {
		case "double": mv.visitInsn(Opcodes.F2D);
			return;
		case "int":    mv.visitInsn(Opcodes.F2I);
			return;
		case "long":   mv.visitInsn(Opcodes.F2L);
			return;
		}
	}

	private static void castFromLong(String to, MethodVisitor mv) {
		switch (to) {
		case "double":
			mv.visitInsn(Opcodes.L2D);
			return;
		case "int":
			mv.visitInsn(Opcodes.L2I);
			return;
		case "float":
			mv.visitInsn(Opcodes.L2F);
			return;
		}
	}

}
