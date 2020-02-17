package fr.umlv.project.feature;

import java.util.Objects;

import org.objectweb.asm.ClassVisitor;
import org.objectweb.asm.Opcodes;

import fr.umlv.project.feature.concatenationFeature.RetroConcat;
import fr.umlv.project.feature.lambdaFeature.RetroLambda;
import fr.umlv.project.feature.nestFeature.RetroNestMembers;

public enum EnumFeature {
	TryWith("try-with-resources", Opcodes.V1_7), Lambda("lambda", Opcodes.V1_8), Concat("concatenation", Opcodes.V9),
	Nest("nestmates", Opcodes.V11), Record("record", 58);

	private final String enumName;
	private final int versionAdded;

	private EnumFeature(String enumName, int version) {
		this.enumName = enumName;
		this.versionAdded = version;
	}

	public static EnumFeature getEnumByName(String name) {
		Objects.requireNonNull(name);
		for (EnumFeature feature : EnumFeature.values()) {
			if (feature.enumName.equals(name)) {
				return feature;
			}
		}
		return null;
	}

	public static boolean requireBackPort(EnumFeature feature, int targetVersion) {
		Objects.requireNonNull(feature);
		return targetVersion < feature.versionAdded;
	}

	public static ClassVisitor getCvTransformer(EnumFeature feature, int api, ClassVisitor cv, ClassFeatureAnalyzer cfa, String className, int targetVersion) {
		switch (feature) {
		case Lambda:  return new RetroLambda(api, cv, targetVersion);
		case Concat:  return new RetroConcat(api, cv, cfa.getConcatClassesInfo(className));
		case Nest:    return new RetroNestMembers(api, cv, cfa.getNestClassesInfo(), cfa.getNestClassesSynTetic());
		case TryWith: return cv;
		case Record:  return cv;
		default:
			throw new IllegalArgumentException("Feature not recognized !");
		}
	}
}