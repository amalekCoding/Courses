package fr.umlv.project.parser;

import java.util.Arrays;
import java.util.HashSet;
import java.util.List;
import java.util.Objects;
import java.util.Set;
import java.util.function.Predicate;
import java.util.regex.Pattern;
import java.util.stream.Collectors;

import fr.umlv.project.feature.EnumFeature;

public class OptionParser {


	// validate options pattern
	private static boolean isValidOption(String line) {
		var pattern = Pattern.compile("((-target(\\s--force)?(\\s(\\d+)))(\\s-info)?(\\s-features(\\s\\[[a-zA-Z,]*\\]))?(\\s-help)?"
				+ "|(-target(\\s--force)?(\\s([5-9]|1[0-3])))?(\\s?-info)(\\s-features(\\s\\[[a-zA-Z,]*\\]))?(\\s-help)?"
				+ "|(-target(\\s--force)?(\\s([5-9]|1[0-3])))?(\\s-info)?(\\s-features(\\s\\[[a-zA-Z,]*\\]))?(\\s?-help))");
		
		var match = pattern.matcher(line);
		return match.matches();
	}
	
	public static List<String> getOptions(String[] args) {
		Objects.requireNonNull(args);
		return Arrays.stream(args).filter(e -> e.startsWith("-") || e.startsWith("[") || Character.isDigit(e.charAt(0))).collect(Collectors.toList());
	}
	
	public static boolean checkIfValidOption(List<String> options) {
		Objects.requireNonNull(options);
		var line = options.stream().collect(Collectors.joining(" "));
		return isValidOption(line);
	}
	


	
	private static boolean isCommandPresent(List<String> options, String command) {
		if (isValidOption(options.stream().collect(Collectors.joining(" ")))) {
			if(options.contains(command)) {
				return true;
			}
		}
		return false;
	}
	
	public static boolean mustPrintInfo(List<String> options) {
		return isCommandPresent(Objects.requireNonNull(options), "-info");
	}
	public static boolean mustPrintHelp(List<String> options) {
		return isCommandPresent(Objects.requireNonNull(options), "-help");
	}
	public static boolean mustBackPortSpecificFeatures(List<String> options) {
		return isCommandPresent(Objects.requireNonNull(options), "-features");
	}
	public static boolean mustRetroVersion(List<String> options) {
		return isCommandPresent(Objects.requireNonNull(options), "-target");
	}
	public static boolean mustTransform(List<String> options) {
		return isCommandPresent(Objects.requireNonNull(options), "--force");
	}

	
	public static Set<EnumFeature> getLstFeaturesToBackPort(List<String> options) {
		if(mustBackPortSpecificFeatures(Objects.requireNonNull(options))) {
			Set<EnumFeature> setFeatures = new HashSet<>();
			var features = options.get(options.indexOf("-features") + 1);
			var lstFeatures = features.replace("[", "").replace("]", "").split(",");
			for(String feature : lstFeatures) {
				if(EnumFeature.getEnumByName(feature) == null) {
					throw new IllegalArgumentException("Backporting the feature: (" +feature+") not supported yet ! You can request it.");
				}
				setFeatures.add(EnumFeature.getEnumByName(feature));
			}
			return setFeatures;
		}
		throw new IllegalArgumentException("Did not ask to backport features.");
	}
	
	public static int getVersionTarget(List<String> options) {
		Objects.requireNonNull(options);
		if(mustRetroVersion(options)) {
			if (options.contains("--force")) {
				return Integer.parseInt(options.get(options.indexOf("--force") + 1));
			}
			return Integer.parseInt(options.get(options.indexOf("-target") + 1));
		}
		throw new IllegalStateException("Doesn't specify the version target.");
	}
	
	
	
	private static Set<String> getLstFilesGiven(String[] args, List<String> options) {
		return ExtractUtil.parse(Arrays.stream(args).skip(options.size()).collect(Collectors.toList()));

	}

	private static Set<String> getFilesNamesWithPredicate(String[] args, List<String> options, Predicate<String> predicate) {
		if (checkIfValidOption(options)) {
			return getLstFilesGiven(args, options).stream().filter(predicate).collect(Collectors.toSet());
		}
		throw new IllegalArgumentException();
	}
		
	public static Set<String> getClassFileNames(String[] args, List<String> options) {
		Objects.requireNonNull(args);
		Objects.requireNonNull(options);
		return getFilesNamesWithPredicate(args, options, fileName -> fileName.endsWith(".class"));
	}
	
	
	public static Set<String> getJarFileNames(String[] args, List<String> options) {
		Objects.requireNonNull(args);
		Objects.requireNonNull(options);
		return getFilesNamesWithPredicate(args, options, fileName -> fileName.endsWith(".jar"));
	}

}
