package fr.umlv.roto;

import java.awt.Graphics2D;
import java.awt.Rectangle;
import java.util.Objects;

abstract class AbstractEntity implements Entity {
	private final Rectangle hitbox;
	private boolean isAlive;

	public AbstractEntity(Rectangle hitbox) {
		this.hitbox = Objects.requireNonNull(hitbox);
		this.isAlive = true;
	}

	public boolean intersects(Entity e) {
		Objects.requireNonNull(e);
		return this.isAlive && e.isAlive() && this.hitbox.intersects(e.hitbox());
	}

	public Rectangle hitbox() {
		return new Rectangle(this.hitbox);
	}

	@Override
	public void kill() {
		this.isAlive = false;
	}

	@Override
	public boolean isAlive() {
		return isAlive;
	}

	void moveTo(int x, int y) {
		this.hitbox.x = x;
		this.hitbox.y = y;
	}

	@Override
	public abstract void handleCollision(Entity e);

	@Override
	public abstract void draw(Graphics2D graph, double zoom);
}
