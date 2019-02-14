package fr.umlv.roto;

import java.awt.Color;
import java.awt.Graphics2D;
import java.awt.Rectangle;
import java.util.Objects;

public class Wall extends AbstractEntity {
	private boolean canDie;

	public Wall(Rectangle hitbox) {
		super(Objects.requireNonNull(hitbox));
		canDie = false;
	}
	
	public void setCanBeKilled() {
		canDie = true;
	}

	@Override
	public void draw(Graphics2D graph, double zoom) {
		Objects.requireNonNull(graph);
		if (zoom < 1) {
			throw new IllegalArgumentException("Invalid zoom level");
		}

		if (this.isAlive()) {
			var hitbox = this.hitbox();

			graph.setColor(Color.BLACK);
			graph.drawRect((int)(hitbox.x * zoom), (int)(hitbox.y * zoom), (int)(hitbox.width * zoom), (int)(hitbox.height * zoom));
			graph.fillRect((int)(hitbox.x * zoom), (int)(hitbox.y * zoom), (int)(hitbox.width * zoom), (int)(hitbox.height * zoom));
		}
	}
	
	@Override
	public void handleCollision(Entity e) {
		Objects.requireNonNull(e);
		if(this == e || !this.intersects(e)) {
			return;
		}
		if(e instanceof Wall) {
			/* Walls are allowed to collide freely. */
		}
		else if(e instanceof Player) {
			/* If a player collides with a wall, it may reset its jump. In all cases a player cannot cross a wall. */
			/* The player knows how to handle this situation better. */
			e.handleCollision(this);
		}
		else if(e instanceof Sword) {
			/* If a sword collides with a wall, the sword dies and the wall too if can be killed. */
			if (canDie) {
				this.kill();
			}
			e.kill();
		}
	}
}
