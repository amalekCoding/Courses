package fr.umlv.project.feature.recordFeature;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.HashSet;
import java.util.List;
import java.util.Objects;
import java.util.Set;

import org.objectweb.asm.ClassReader;
import org.objectweb.asm.ClassVisitor;

import fr.umlv.project.feature.FeatureCollector;

public class CollectClassesWithRecord implements FeatureCollector {

	private final Collection<byte[]> bytesContents;
	private final Set<String> recordClassNames = new HashSet<>();
	private final int api;

	
	private CollectClassesWithRecord(int api, Collection<byte[]> bytesContents) {
		this.bytesContents = Collections.unmodifiableCollection(bytesContents);
		this.api = api;
	}

	public static CollectClassesWithRecord collectClassesWithRecordFactory(int api,
			Collection<byte[]> bytesContents) {
		var recordCollector = new CollectClassesWithRecord(api, Objects.requireNonNull(bytesContents));
		recordCollector.collect();
		return recordCollector;
	}
	
	private void collect() {
		bytesContents.forEach(byteContent -> {
			ClassReader cr = new ClassReader(byteContent);
			cr.accept(new DetectRecord(api), 0);
		});
	}


	@Override
	public void printFeature() {
		recordClassNames.forEach(className ->
			System.out.println("RECORD at " + className)
		);
	}
	
	@Override
	public List<String> getFeaturesMessage() {
		List<String> lst = new ArrayList<>();
		recordClassNames.forEach(className -> 
			lst.add("RECORD at " + className)
		);
		return lst;
	}
	
	

	@Override
	public Set<String> getClassesNamesFeature() {
		return new HashSet<>(recordClassNames);
	}
	

	private class DetectRecord extends ClassVisitor {

		private DetectRecord(int api) {
			super(api);
		}
		
		@Override
		public void visit(int version, int access, String name, String signature, String superName,
				String[] interfaces) {
			
			if(superName.equals("java/lang/Record")) {
				recordClassNames.add(name);
			}
			super.visit(version, access, name, signature, superName, interfaces);
		}
		
		
		
	}
	

}
