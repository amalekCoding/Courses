package fr.umlv.project.feature.nestFeature;

import java.util.Objects;

public class MethodInfo implements ClassAtribute {

	private final String name;
	private final String desc;

	public MethodInfo(String name, String desc) {
		this.name = Objects.requireNonNull(name);
		this.desc = Objects.requireNonNull(desc);
	}

	@Override
	public boolean equals(Object obj) {
		if (obj instanceof MethodInfo) {
			MethodInfo method = (MethodInfo) obj;
			if (method.name.equals(name) & method.desc.equals(desc)) {
				return true;
			}
		}
		return false;
	}

	@Override
	public int hashCode() {
		return name.hashCode() ^ desc.hashCode();
	}

	public String getName() {
		return name;
	}

	public String getDesc() {
		return desc;
	}
}
