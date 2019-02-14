package fr.umlv.roto;

import java.util.List;

public interface Moveable extends Entity {
	void move(List<Wall> walls, Vector2D gravity);
}
