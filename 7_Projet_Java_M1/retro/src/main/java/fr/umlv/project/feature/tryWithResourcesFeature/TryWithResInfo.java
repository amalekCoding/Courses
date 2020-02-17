package fr.umlv.project.feature.tryWithResourcesFeature;

import java.util.Objects;

public class TryWithResInfo {
	
	
	private final String classOwner;
	private final String methodName;
	private final String methodDesc;
	private final String owner;
	private final int line;
	
	
	
	public TryWithResInfo(String classOwner, String methodName, String methodDesc, String owner, int line) {
		this.classOwner = Objects.requireNonNull(classOwner);
		this.methodName = Objects.requireNonNull(methodName);
		this.methodDesc = Objects.requireNonNull(methodDesc);
		this.owner = Objects.requireNonNull(owner);
		if(line < 0) {
			throw new IllegalArgumentException("line can't be < 0 !");
		}
		this.line = line;
	}
	
	
	
	@Override
	public String toString() {
		return "TRY_WITH_RESOURCES at " + classOwner+"."+methodName +methodDesc+ " ("+classOwner+".java:"+line+"): try-with-resources on "+owner;
	}

}
