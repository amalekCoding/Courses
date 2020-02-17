package fr.umlv.project.feature.nestFeature;

public class FieldInfo implements ClassAtribute {

	private final String name;
	private final String desc;

	public FieldInfo(String name, String desc) {
		this.name = name;
		this.desc = desc;
	}

	@Override
	public boolean equals(Object obj) {
		if (obj instanceof FieldInfo) {
			FieldInfo field = (FieldInfo) obj;
			if (field.name.equals(name)) {
				return true;
			}
		}
		return false;
	}

	@Override
	public int hashCode() {
		return name.hashCode();
	}

	public String getName() {
		return name;
	}

	public String getDesc() {
		return desc;
	}
}
