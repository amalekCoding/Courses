package fr.umlv.project.feature;

import java.util.List;
import java.util.Set;

/**
 * This interface imposes for each class that implements it, to be able to print
 * all the information of the feature collected or to give it and to be able to give all the
 * classes names that implement the feature.
 */
public interface FeatureCollector {

	public void printFeature();

	public List<String> getFeaturesMessage();

	public Set<String> getClassesNamesFeature();
		
}
