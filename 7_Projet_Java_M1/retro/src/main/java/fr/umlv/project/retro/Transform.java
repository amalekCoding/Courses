package fr.umlv.project.retro;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.HashMap;
import java.util.Map;
import java.util.Objects;
import java.util.Set;
import java.util.function.Consumer;

import org.objectweb.asm.ClassReader;
import org.objectweb.asm.ClassVisitor;
import org.objectweb.asm.ClassWriter;

import fr.umlv.project.feature.ClassFeatureAnalyzer;
import fr.umlv.project.feature.EnumFeature;
import fr.umlv.project.parser.WriteUtil;

/**
 * A class that allow you to transform a Java Byte Code.
 */
public class Transform {

	/** Private constructor */
	public Transform() {
	}
	
	
	private static void test(ClassFeatureAnalyzer cfa, Set<EnumFeature> setFeatures, boolean mustBackport, int targetVersion) {
		for (EnumFeature feature : EnumFeature.values()) {
			if(cfa.getListOfClassWithFeature(feature).size() != 0 && !setFeatures.contains(feature) && EnumFeature.requireBackPort(feature, targetVersion) && !mustBackport) {
				System.err.println("\nThere is the feature : ("+feature+") in the program.");
				System.err.println("If you really want to backport that feature to a version where it wasn't introduced yet please use the option --force");
				System.err.println("and add that features in the lst of features to transform in the option -features or don't use the -features options to backport all the features.");
			}
		}

	}

	
	public static Map<String, byte[]> backportFeatures(int api, Map<String, byte[]> classMap, ClassFeatureAnalyzer cfa,
			Set<EnumFeature> setFeatures, int targetVersion, boolean mustBackport) {
		Objects.requireNonNull(classMap);
		Objects.requireNonNull(cfa);
		Objects.requireNonNull(setFeatures);
		test(cfa, setFeatures, mustBackport, targetVersion);
		for (EnumFeature feature : setFeatures) {
			cfa.getListOfClassWithFeature(feature).forEach(className -> {
				classMap.replace(className+".class", transformFeature(api,
						new ClassReader(classMap.get(className + ".class")), cfa, feature, targetVersion, mustBackport, setFeatures));
			});
			if (feature.equals(EnumFeature.Lambda) && cfa.getListOfClassWithFeature(feature).size() != 0 && (mustBackport | EnumFeature.requireBackPort(feature, targetVersion))) {
				var lambdaFilePath = "mybinLambda/";
				classMap.putAll(transformLambdaFilesGeneratedToMap(classMap, lambdaFilePath));
				WriteUtil.deleteDirectory(lambdaFilePath);
				new File(lambdaFilePath).delete();
			}
		}
		return new HashMap<>(classMap);
	}

	public static Map<String, byte[]> lowerByteCodeVersion(int api, Map<String, byte[]> classMap, int targetVersion) {
		Objects.requireNonNull(classMap);
		classMap.forEach((className, bytesContent) -> {
			classMap.replace(className, transform(new ClassReader(classMap.get(className)), (newCv) -> {
				newCv = new LowerByteCodeVersion(api, newCv, targetVersion);
				return newCv;
			}));
			
		});
		return new HashMap<>(classMap);
	}
	
	private static Map<String, byte[]> transformLambdaFilesGeneratedToMap(Map<String, byte[]> classMap, String lambdaFilePath) {
		try {
			Files.walk(Paths.get(lambdaFilePath))
					.filter(file -> Files.isRegularFile(file) & file.toString().endsWith(".class"))
					.forEach(name -> classMap.put(name.toString().replace(lambdaFilePath, ""),
							transformFileToByte(name.toString())));
		} catch (IOException e) {
			throw new IllegalStateException("Couldn't transform the lambda files generated to map. ", e);
		}

		return classMap;
	}

	public static byte[] transformFileToByte(String pathName) {
		Objects.requireNonNull(pathName);
		try (FileInputStream byteContents = new FileInputStream(pathName)) {
			var reader = new ClassReader(byteContents);
			var writer = new ClassWriter(reader, 0);
			reader.accept(writer, 0);
			return writer.toByteArray();
		} catch (IOException e) {
			throw new IllegalStateException("Couldn't transform the " + pathName + " file." +  e);
		}
	}

	private static ClassVisitor parseFeature(int api, String className, ClassVisitor cv, ClassFeatureAnalyzer cfa,
			EnumFeature feature, int targetVersion, boolean mustBackport, Set<EnumFeature> setFeatures) {
		boolean backport = mustBackport || (setFeatures.contains(feature) && EnumFeature.requireBackPort(feature, targetVersion));
		if(!backport) {
			return cv;
		}
		return EnumFeature.getCvTransformer(feature, api, cv, cfa, className, targetVersion);
	}

	
	private static byte[] transformFeature(int api, ClassReader reader, ClassFeatureAnalyzer ca, EnumFeature feature,
			int targetVersion, boolean mustBackport, Set<EnumFeature> setFeatures) {
		return transform(reader, (newCv) -> {
			newCv = parseFeature(api, reader.getClassName(), newCv, ca, feature, targetVersion, mustBackport, setFeatures);
			return newCv;
		});

	}

	private interface ClassVisitorTransformation {
		ClassVisitor transform(ClassVisitor newCv);
	}

	private static byte[] transform(ClassReader reader, ClassVisitorTransformation funcTranformer) {
		return transform(reader.getClassName(), cv -> reader.accept(cv, 0), funcTranformer);
	}

	private static byte[] transform(String className, Consumer<ClassVisitor> reader,
			ClassVisitorTransformation funcTranformer) {
			ClassWriter writer = new ClassWriter(ClassWriter.COMPUTE_MAXS | ClassWriter.COMPUTE_FRAMES);
			ClassVisitor newCv = writer;

			newCv = funcTranformer.transform(newCv);
			
			reader.accept(newCv);
			return writer.toByteArray();
	}
	
}
