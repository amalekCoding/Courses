package fr.umlv.project.retro;

import java.nio.file.Path;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.stream.Collectors;

import javax.ws.rs.Consumes;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.PUT;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;

import org.objectweb.asm.Opcodes;

import fr.umlv.project.feature.ClassFeatureAnalyzer;
import fr.umlv.project.feature.EnumFeature;
import fr.umlv.project.parser.ExtractUtil;
import fr.umlv.project.parser.OptionParser;
import fr.umlv.project.parser.WriteUtil;






public class Main {

	private static final int API_VERSION = Opcodes.ASM7;

	private static int getJavaCodeVersion(int version) {
		if (version < 5 || version > 14) {
			throw new IllegalArgumentException(
					"Can't backport to this version : V" + version + " only down to V" + 5 + ".");
		}
		return Opcodes.V1_5 + (version - 5);
	}

	private static Map<String, byte[]> analyzeAndTransform(Map<String, byte[]> classMap, int targetVersion, boolean needTransform, boolean mustTransform, boolean mustPrintInfo, Set<EnumFeature> setFeatures, Path input, Path output) {
		ClassFeatureAnalyzer cfa = ClassFeatureAnalyzer.classFeatureAnalyzerFactory(API_VERSION, classMap);
		if (mustPrintInfo) {
			cfa.printFeatures();
		}
		if (needTransform) {
			if (setFeatures.size() != 0) {
				classMap = Transform.backportFeatures(API_VERSION, classMap, cfa, setFeatures, targetVersion, mustTransform);
			}
			classMap = Transform.lowerByteCodeVersion(API_VERSION, classMap, targetVersion);
		}
		return classMap;
	}
	

	private static void runForClasses(Map<String, byte[]> classMap, int targetVersion, boolean needTransform, boolean mustTransform, boolean mustPrintInfo, Set<EnumFeature> setFeatures, Set<String> setClasses) {
		if (setClasses.size() != 0) {
			classMap = ExtractUtil.extractAllClassesFromFilesToMap(setClasses);
			classMap = analyzeAndTransform(classMap, targetVersion, needTransform, mustTransform, mustPrintInfo, setFeatures, Path.of(""), Path.of(""));
			if (needTransform) {
				WriteUtil.writeNewClasses(classMap);
			}
		}
	}
	
	private static void run(int targetVersion, boolean needTransform, boolean mustTransform, boolean mustPrintInfo, Set<EnumFeature> setFeatures, Set<String> setJars, Set<String> setClasses) {
		Map<String, byte[]> classMap = new HashMap<>();
		for (var jarName : setJars) {
			classMap = ExtractUtil.extractFileToMap(Path.of(jarName));
			classMap = analyzeAndTransform(classMap, targetVersion, needTransform, mustTransform, mustPrintInfo,
					setFeatures, Path.of(jarName), Path.of(jarName));
			if (needTransform) {
				WriteUtil.writeNewJar(API_VERSION, classMap, Path.of(jarName), Path.of(jarName.substring(0, jarName.indexOf("."))+"1.jar"));
			}
		}
		runForClasses(classMap, targetVersion, needTransform, mustTransform, mustPrintInfo, setFeatures, setClasses);
	}
	

	
	
	

	private static void ifDoMustPrintHelp(List<String> options) {
		if (OptionParser.mustPrintHelp(options)) {
			System.out.println(
					"Usage : [[-info] | [-help] | -target [--force] version [-info] [-features [features1,features2,...]]  [files...]");
			System.out.println("");
			return;
		}
	}
	

	// Exemple of args = new String[] {"-target", "--force", "7", "-info", "-features", "[lambda,concatenation]", "dir/file1.class", "file2.jar"};
	public static void main(String[] args) {
		
		List<String> options = OptionParser.getOptions(args);

		if (!OptionParser.checkIfValidOption(options)) {
			throw new IllegalArgumentException("Invalid options !");
		}

		ifDoMustPrintHelp(options);

		int targetVersion = -1;
		Set<EnumFeature> setFeatures = new HashSet<>();
		var mustPrintInfo = OptionParser.mustPrintInfo(options);
		;
		var needTransform = OptionParser.mustRetroVersion(options);
		var mustTransformForce = OptionParser.mustTransform(options);

		if (needTransform) {
			targetVersion = getJavaCodeVersion(OptionParser.getVersionTarget(options));
			if (OptionParser.mustBackPortSpecificFeatures(options)) {
				setFeatures = OptionParser.getLstFeaturesToBackPort(options);
			} else if (mustTransformForce) {
				setFeatures = Arrays.asList(EnumFeature.values()).stream().collect(Collectors.toSet());
			}
		}
		var setClassesNames = OptionParser.getClassFileNames(args, options);
		var setJarsNames = OptionParser.getJarFileNames(args, options);

		run(targetVersion, needTransform, mustTransformForce, mustPrintInfo, setFeatures, setJarsNames,
				setClassesNames);
	}

}
