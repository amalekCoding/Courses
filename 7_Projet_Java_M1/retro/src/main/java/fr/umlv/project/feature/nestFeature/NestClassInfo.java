package fr.umlv.project.feature.nestFeature;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Objects;
import java.util.Optional;
import java.util.Set;
import java.util.StringJoiner;

public class NestClassInfo {

    private final Set<ClassAtribute> privateFieldsOrMethods = new HashSet<>();
    private final List<String> nestMembers = new ArrayList<>();
    private Optional<String> nestHost = Optional.empty();	
    private final String className;
    
    
    public NestClassInfo(String className) {
    	this.className = Objects.requireNonNull(className);
    }
    
    
	public boolean contains(ClassAtribute privateFieldsOrMethod) {
    	return privateFieldsOrMethods.contains(Objects.requireNonNull(privateFieldsOrMethod));
    }
	
    public void addPrivateAttribute(ClassAtribute ca) {
    	privateFieldsOrMethods.add(Objects.requireNonNull(ca));
    }
    
    public void addNestMembers(String name) {
    	nestMembers.add(Objects.requireNonNull(name));
    }
    
    public void addNestHost(String name) {
    	nestHost = Optional.of(Objects.requireNonNull(name));
    }
    
    private boolean isNestHost() {
    	if(nestMembers.size() != 0) {
    		if(nestHost.isEmpty()) {
    			return true;    			
    		}
    		throw new IllegalStateException("Is already a NestHost can't have a NestHost.");
    	}
    	return false;
    }
    
    private String getClassNameWithoutPackage(String name) {
    	return name.substring(name.lastIndexOf("/")+1);
    }

    @Override
    public String toString() {
    	if(isNestHost()) {
    		StringJoiner sj = new StringJoiner(", ", "[", "]");
    		for(String name : nestMembers) {
    			sj.add(name);
    		}
    		return "NESTMATES at " + className + " ("+className+".java): nest host " + className+ " members " + sj.toString();
    	}
		return "NESTMATES at " +className+ " ("+getClassNameWithoutPackage(nestHost.get())+".java): nestmate of " + nestHost.get();
    }

}
