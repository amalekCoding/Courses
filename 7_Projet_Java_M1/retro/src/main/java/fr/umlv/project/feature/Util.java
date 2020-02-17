package fr.umlv.project.feature;

import java.util.ArrayList;
import java.util.List;
import java.util.Objects;

import org.objectweb.asm.Opcodes;
import org.objectweb.asm.Type;

public class Util {

	/**
	 * From a {@code Type} return the good load instruction associated.
	 * <p>
	 * <b>Exemple :</b>
	 * </p>
	 * for a type
	 * <p>
	 * - "Ljava/lang/String" return : Opcodes.ALOAD.
	 * </p>
	 * <p>
	 * - "I" return : Opcodes.ILOAD.
	 * </p>
	 * 
	 * @param type The type of a variable.
	 * 
	 * @return The Opcodes load instruction associated.
	 */
	public static int getLoadOpcodesFromType(Type type) {
		Objects.requireNonNull(type);
		
		char charType = type.getDescriptor().charAt(0);
		if (getLstPrimaryTypeForOpcodesWithI().contains(charType + "")) {
			return Opcodes.ILOAD;
		}
		switch (charType) {
		case 'D': return Opcodes.DLOAD;
		case 'F': return Opcodes.FLOAD;
		case 'J': return Opcodes.LLOAD;
		case 'L': return Opcodes.ALOAD;
		case '[': return Opcodes.ALOAD;
		default:
			throw new ClassFormatError("Invalid method signature: " + type.getDescriptor());
		}
	}
	
	
	private static List<String> getLstPrimaryTypeForOpcodesWithI() {
		return new ArrayList<>(List.of("B", "C", "Z", "S", "I"));
	}
	
	public static List<String> getLstPrimaryType() {
		return new ArrayList<>(List.of("B", "C", "Z", "S", "I", "J", "D"));
	}
	

	/**
	 * From a {@code Type} return the good Opcodes.RETURN instruction associated.
	 * <p>
	 * <b>Exemple :</b>
	 * </p>
	 * for a type
	 * <p>
	 * - "Ljava/lang/String" return : Opcodes.ARETURN.
	 * </p>
	 * <p>
	 * - "I" return : Opcodes.IRETURN.
	 * </p>
	 * 
	 * @param type The type of a variable.
	 * 
	 * @return The Opcodes return instruction associated.
	 */
	public static int getReturnOpcodesFromType(Type type) {
		Objects.requireNonNull(type);

		char charType = type.getDescriptor().charAt(0);
		if (getLstPrimaryTypeForOpcodesWithI().contains(charType + "")) {
			return Opcodes.IRETURN;
		}
		switch (charType) {
		case 'V': return Opcodes.RETURN;
		case 'D': return Opcodes.DRETURN;
		case 'F': return Opcodes.FRETURN;
		case 'J': return Opcodes.LRETURN;
		case 'L': return Opcodes.ARETURN;
		case '[': return Opcodes.ARETURN;
		default:
			throw new ClassFormatError("Invalid method signature: " + type.getDescriptor());
		}
	}

}
