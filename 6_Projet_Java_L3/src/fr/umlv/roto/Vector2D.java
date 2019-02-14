package fr.umlv.roto;

import java.util.Objects;

public class Vector2D {
	private final float dx;
	private final float dy;
	
	public Vector2D(float dx, float dy) {
		this.dx = dx;
		this.dy = dy;
	}
	
	public Vector2D(float xa, float ya, float xb, float yb) {
		this(xa - xb, ya - yb);
	}
	
	public Vector2D(Vector2D v) {
		this(Objects.requireNonNull(v).dx, Objects.requireNonNull(v).dy);
	}

	
	public Vector2D(Vector2D v, double rotation) {
		this(rotateVector(Objects.requireNonNull(v), rotation));
	}
	
	private float magnitude() {
		return (float)Math.sqrt(dx * dx + dy * dy);
	}
	
	private float phase() {
		return (float)Math.atan2(dy, dx);
	}

	private static Vector2D rotateVector(Vector2D v, double rotation) {
		var mag = v.magnitude();
		rotation += v.phase();
	
		return new Vector2D(mag * (float)Math.cos(rotation), mag * (float)Math.sin(rotation));
	}

	public float dx() {
		return this.dx;
	}
	
	public float dy() {
		return this.dy;
	}
}
