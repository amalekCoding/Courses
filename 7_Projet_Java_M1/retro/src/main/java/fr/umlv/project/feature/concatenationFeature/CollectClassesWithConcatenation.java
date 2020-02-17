package fr.umlv.project.feature.concatenationFeature;

import java.util.ArrayList;
import java.util.Collection;

import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Objects;
import java.util.Set;

import org.objectweb.asm.ClassReader;
import org.objectweb.asm.ClassVisitor;
import org.objectweb.asm.Handle;
import org.objectweb.asm.Label;
import org.objectweb.asm.MethodVisitor;

import fr.umlv.project.feature.FeatureCollector;



public class CollectClassesWithConcatenation implements FeatureCollector {

	private final int api;

	/** The bytes contents of all the classes that we want to detect the feature. */
	private final Collection<byte[]> bytesContents;
	
	/** The name of the classes with their informations. */
	private final Map<String, Map<ConcatInfo, String>> concatClassesMap = new HashMap<>();


	
	/** Private constructor. */
	private CollectClassesWithConcatenation(int api, Collection<byte[]> bytesContents) {
		this.bytesContents = Collections.unmodifiableCollection(bytesContents);
		this.api = api;
	}
	
	
	public static CollectClassesWithConcatenation collectClassesWithConcatenationFactory(int api, Collection<byte[]> bytesContents) {
		Objects.requireNonNull(bytesContents);
		var concatCol = new CollectClassesWithConcatenation(api, bytesContents);
		concatCol.collect();
		return concatCol;
	}

	private void collect() {
		bytesContents.forEach(byteContent -> {
			var cr = new ClassReader(byteContent);
			cr.accept(new DetectConcatFeature(api), 0);
		});
	}

	public Map<String, Map<ConcatInfo, String>> getConcatClassesMap() {
		return new HashMap<>(concatClassesMap);
	}
	
	
	public Map<ConcatInfo, String> getConcatInfos(String className) {
		Objects.requireNonNull(className);
		return new HashMap<>(concatClassesMap.get(className));
	}
	
	@Override
	public Set<String> getClassesNamesFeature() {
		return new HashSet<>(concatClassesMap.keySet());
	}
	
	@Override
	public void printFeature() {
		concatClassesMap.forEach((className, concatInfosMap) -> 
			concatInfosMap.keySet().forEach(concatInfo -> System.out.println(concatInfo))
		);
	}
	
	@Override
	public List<String> getFeaturesMessage() {
		List<String> lst = new ArrayList<>();
		concatClassesMap.forEach((className, concatInfosMap) -> {
			concatInfosMap.keySet().forEach(concatInfo -> lst.add(concatInfo.toString()));
		});
		return lst;
	}
	

	
	
	private class DetectConcatFeature extends ClassVisitor {
		
		private DetectConcatFeature(int api) {
			super(api);
		}

		private String classOwnerName;
		private String methodNameOwner;
		private String methodDescOwner;
		
		/** For each method count the number of concatenation feature present
		 *  that will be needed to be replaced if backported.  */
		private int indiceNewMethodSynthNeeded;
		 

         @Override
         public void visit(int version, int access, String name, String signature, String superName, String[] interfaces) {
             this.classOwnerName = name;
         }

         @Override
         public MethodVisitor visitMethod(int access, String name, String desc, String signature, String[] exceptions) {
         	methodNameOwner = name;
         	methodDescOwner = desc;
        	MethodVisitor mv = super.visitMethod(access, name, desc, signature, exceptions);
         	return new MethodVisitor(api, mv) {
         		
         		private int line;
         		
         		@Override
         		public void visitCode() {
         			// We reinitialize the count for each new function visited.
         			indiceNewMethodSynthNeeded = 0;
         			super.visitCode();
         		}
         		
         		@Override
         		public void visitLineNumber(int line, Label start) {
         			this.line = line;
         			super.visitLineNumber(line, start);
         		}
         		
         		@Override
         		public void visitInvokeDynamicInsn(String name, String descriptor, Handle bootstrapMethodHandle,
         				Object... bootstrapMethodArguments) {
         			if(bootstrapMethodHandle.getOwner().equals("java/lang/invoke/StringConcatFactory") & name.equals("makeConcatWithConstants")) {
         				String stringPattern = (String) bootstrapMethodArguments[0];
         				ConcatInfo ci = new ConcatInfo(classOwnerName, methodNameOwner, methodDescOwner, descriptor, stringPattern, "%1", line);
         				addTheConcatIfDetected(ci, getSyntheticConcatMethodNameToCreate());
         			}
         			super.visitInvokeDynamicInsn(name, descriptor, bootstrapMethodHandle, bootstrapMethodArguments);
         			
         		}
         		
         		/** The name of the new function that will be created to replace the concatenation
         		 *  that uses InvokeDynamic with a String Builder if asked for. */
         		private String getSyntheticConcatMethodNameToCreate() {
         			return "concatBuild$"+methodNameOwner+"$"+indiceNewMethodSynthNeeded;
         		}
         		
         		private void addTheConcatIfDetected(ConcatInfo ci, String newSynthName) {
     				if(concatClassesMap.get(classOwnerName) != null) {
     					// If it already called makeConcatWithConstants with a same signature 
     					// we don't have to create a new method each time we just have to call
     					// the method generated with the same signature.
         				if(concatClassesMap.get(classOwnerName).containsKey(ci)) {
         					return;
         				}
     				} else {
     					concatClassesMap.put(classOwnerName, new HashMap<>());
     				}
     				concatClassesMap.get(classOwnerName).put(ci, newSynthName);
     				indiceNewMethodSynthNeeded++;
         		}
         	};
         }  

	}


	
	




}
