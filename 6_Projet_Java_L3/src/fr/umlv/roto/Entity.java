package fr.umlv.roto;

import java.awt.Graphics2D;
import java.awt.Rectangle;

public interface Entity {

	Rectangle hitbox();

	void kill();

	boolean isAlive();

	boolean intersects(Entity e);

	void handleCollision(Entity e);

	void draw(Graphics2D graph, double zoom);

}