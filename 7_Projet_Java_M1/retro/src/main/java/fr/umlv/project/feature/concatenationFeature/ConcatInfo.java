package fr.umlv.project.feature.concatenationFeature;

import java.util.Objects;

public class ConcatInfo {

	private final String classOwnerName;
	private final String methodOwnerName;
	private final String methodOwnerDesc;
	private final String concatDesc;
	private final String stringPatern;
	private final String newDelimiterPaternWanted;
	private final int line;

	
	
	public ConcatInfo(String classOwnerName, String methodOwnerName, String methodOwnerDesc, String concatDesc,
			String stringPatern, String newDelimiterPaternWanted, int line) {
		this.methodOwnerName = Objects.requireNonNull(methodOwnerName);
		this.methodOwnerDesc = Objects.requireNonNull(methodOwnerDesc);
		this.concatDesc = Objects.requireNonNull(concatDesc);
		this.stringPatern = Objects.requireNonNull(stringPatern);
		this.newDelimiterPaternWanted = Objects.requireNonNull(newDelimiterPaternWanted);
		this.classOwnerName = Objects.requireNonNull(classOwnerName);
		if (line < 0) {
			throw new IllegalArgumentException("Line of the concatenation can't be < 0.");
		}
		this.line = line;
	}


	@Override
	public boolean equals(Object obj) {
		if (obj instanceof ConcatInfo) {
			ConcatInfo concatInfo = (ConcatInfo) obj;
			if (classOwnerName.equals(concatInfo.classOwnerName)
					&& methodOwnerName.equals(concatInfo.methodOwnerName)) {
				if (concatDesc.equals(concatInfo.concatDesc) && methodOwnerDesc.equals(concatInfo.methodOwnerDesc)) {
					return true;
				}
			}
		}
		return false;
	}

	@Override
	public int hashCode() {
		return classOwnerName.hashCode() ^ methodOwnerName.hashCode() ^ methodOwnerDesc.hashCode()
				^ concatDesc.hashCode();
	}



	public String getConcatDesc() {
		return concatDesc;
	}
	public String getNewDelimiter() {
		return newDelimiterPaternWanted;
	}
	public String getNewPaternWanted() {
		return stringPatern.replace("\u0001", newDelimiterPaternWanted);
	}


	@Override
	public String toString() {
		String tokens = getNewPaternWanted();
		return "CONCATENATION at " + classOwnerName+"."+methodOwnerName + methodOwnerDesc + " (" + classOwnerName + ".java:"+line+"): pattern "
				+ tokens.toString();
	}
}