package fr.umlv.project.feature.lambdaFeature;

import java.util.Objects;

public class LambdaInfo {
	
	
	private final String classOwner;
	private final String methodOwner;
	private final String methodOwnerDesc;
	private final String interfaceName;
	private final String interfaceDesc;
	private final String lambdaFunctionCalled;
	private final int line;
	
	public LambdaInfo(String classOwner, String methodOwner, String methodOwnerDesc, String interfaceName, String interfaceDesc, String lambdaFunctionCalled, int line) {
		this.classOwner = Objects.requireNonNull(classOwner);
		this.methodOwner = Objects.requireNonNull(methodOwner);
		this.methodOwnerDesc = Objects.requireNonNull(methodOwnerDesc);
		this.interfaceName = Objects.requireNonNull(interfaceName);
		this.interfaceDesc = Objects.requireNonNull(interfaceDesc);
		this.lambdaFunctionCalled = Objects.requireNonNull(lambdaFunctionCalled);
		if(line < 0) {
			throw new IllegalArgumentException("Invalid number of line.");
		}
		this.line = line;
	}



	@Override
	public String toString() {
		String classOwnerInternalName = classOwner.substring(classOwner.lastIndexOf(".")+1);
		var interfaceDescr = interfaceDesc.replace("(", "").replace(")", "");
		return "LAMBDA at " +classOwner+"."+methodOwner+methodOwnerDesc+" ("+classOwnerInternalName+".java:"+line+"): lambda " + interfaceName+" capture ["+interfaceDescr+"] calling "+ lambdaFunctionCalled.substring(0, lambdaFunctionCalled.length()-4);
	}

}
