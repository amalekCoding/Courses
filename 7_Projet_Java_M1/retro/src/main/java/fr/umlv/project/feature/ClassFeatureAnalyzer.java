package fr.umlv.project.feature;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Objects;
import java.util.Set;

import fr.umlv.project.feature.concatenationFeature.CollectClassesWithConcatenation;
import fr.umlv.project.feature.concatenationFeature.ConcatInfo;
import fr.umlv.project.feature.lambdaFeature.CollectClassesWithLambda;
import fr.umlv.project.feature.nestFeature.ClassAtribute;
import fr.umlv.project.feature.nestFeature.CollectClassesWithNestMembers;
import fr.umlv.project.feature.nestFeature.NestClassInfo;
import fr.umlv.project.feature.recordFeature.CollectClassesWithRecord;
import fr.umlv.project.feature.tryWithResourcesFeature.CollectClassesWithTryWithRes;



/**
 *  Class that allows you to analyze a given Map of classes by detecting and collecting
 *  all the feature of a Map of classes given.
 */
public class ClassFeatureAnalyzer {

	/** Every name of the class associated with it's bytes content. */
	private final Map<String, byte[]> classContentMap;

	/** For each EnumFeature associate it's collector. */
	private final Map<EnumFeature, FeatureCollector> featuresInfos = new HashMap<>();;

	private final Map<String, NestClassInfo> nestClassesInfos = new HashMap<>();
		
	private final Map<String, Map<ConcatInfo, String>> concatClassesMap = new HashMap<>();
	
	private final Map<String, Set<ClassAtribute>> nestClassesThatNeedSyntheticMethod = new HashMap<>();

	
	private final int api;
	

	/**
	 * Private Constructor.
	 * @param classMap: the given Map that associate a className with it's bytes content.
	 */
	private ClassFeatureAnalyzer(Map<String, byte[]> classContentMap, int api) {
		this.classContentMap = Collections.unmodifiableMap(classContentMap);
		this.api = api;
	}
	
	
	/**
	 * ClassFeatureAnalyzerFactory : create a ClassFeatureAnalyzer instance and analyze all the 
	 * classes content and extract all the features informations.
	 * @param api
	 * @param classMap
	 * @return a new instance with the all the informations of the features.
	 */
	public static ClassFeatureAnalyzer classFeatureAnalyzerFactory(int api, Map<String, byte[]> classMap) {
		Objects.requireNonNull(classMap);
		ClassFeatureAnalyzer cfa = new ClassFeatureAnalyzer(classMap, api);
		cfa.analyze();
		return cfa;
	}


	/** Method that allow you to collect all the informations from the classMap. */
	private void analyze() {		
		collectLambdaClasses();
		collectTryWithResClasses();
		collectNestMembersClasses();
		collectConcatClasses();
		collectRecord();
	}
	
	private void collectNestMembersClasses() {
		var nestCollector = CollectClassesWithNestMembers.collectClassesWithNestMembersFactory(api, classContentMap);
		nestClassesInfos.putAll(nestCollector.getNestClassesInfos());
		nestClassesThatNeedSyntheticMethod.putAll(nestCollector.getClassesThatNeedSyntheticMethod());
		featuresInfos.put(EnumFeature.Nest, nestCollector);
	}	
	private void collectConcatClasses() {
		var concatCollector = CollectClassesWithConcatenation.collectClassesWithConcatenationFactory(api, classContentMap.values());
		concatClassesMap.putAll(concatCollector.getConcatClassesMap());
		featuresInfos.put(EnumFeature.Concat, concatCollector);
	}
	private void collectLambdaClasses() {
		var lambdasCollector = CollectClassesWithLambda.collectLambdaFactory(api, classContentMap.values());
		featuresInfos.put(EnumFeature.Lambda, lambdasCollector);
	}
	private void collectTryWithResClasses() {
		var tryWithResCollector = CollectClassesWithTryWithRes.collectClassesWithTryWithResFactory(api, classContentMap.values());
		featuresInfos.put(EnumFeature.TryWith, tryWithResCollector);
	}
	private void collectRecord() {
		var recordCollector = CollectClassesWithRecord.collectClassesWithRecordFactory(api, classContentMap.values());
		featuresInfos.put(EnumFeature.Record, recordCollector);
	}
	
	public FeatureCollector getInfoFeatures(EnumFeature feature) {		
		if(featuresInfos.get(Objects.requireNonNull(feature)) == null) {
			throw new IllegalStateException("Don't have information about that feature : " + feature);
		}
		return featuresInfos.get(feature);
	}
	
	
	public Set<String> getListOfClassWithFeature(EnumFeature feature) {
		return getInfoFeatures(Objects.requireNonNull(feature)).getClassesNamesFeature();
	}


	/** Print all the informations about the features present in the classes given. */
	public void printFeatures() {
		featuresInfos.forEach((feature, featureCollector)-> featureCollector.printFeature()); 
	}
	
	
	public Map<ConcatInfo, String> getConcatClassesInfo(String name) {
		return new HashMap<>(concatClassesMap.get(name));
	}
	
	public Map<String, NestClassInfo> getNestClassesInfo() {
		return new HashMap<>(nestClassesInfos);
	}
	public Map<String, Set<ClassAtribute>> getNestClassesSynTetic() {
		return new HashMap<>(nestClassesThatNeedSyntheticMethod);
	}
	
	
	public List<String> getMessageFeature() {
		List<String> lst = new ArrayList<>();
		featuresInfos.forEach((feature, featureColl) -> lst.addAll(featureColl.getFeaturesMessage()));
		return lst;
	}

}
