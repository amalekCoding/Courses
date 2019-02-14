package fr.umlv.roto;

import java.awt.Color;
import java.awt.Graphics2D;
import java.awt.Rectangle;
import java.util.List;
import java.util.Objects;

public class Sword extends AbstractEntity implements Moveable {
	private static final int SWORDSPEED = 4;
	private final Vector2D direction;
	private final Color color;
	private final Player owner;

	public Sword(Rectangle hitbox, Vector2D direction, Color color, Player owner) {
		super(Objects.requireNonNull(hitbox));
		this.direction = Objects.requireNonNull(direction);
		this.color = Objects.requireNonNull(color);
		this.owner = owner;
	}

	@Override
	public void draw(Graphics2D graph, double zoom) {
		Objects.requireNonNull(graph);
		if (zoom < 1) {
			throw new IllegalArgumentException("Invalid zoom level");
		}

		if (this.isAlive()) {

			var hitbox = this.hitbox();
			var centerX = (hitbox.x + hitbox.width / 2);
			var centerY = (hitbox.y + hitbox.height / 2);

			graph.setColor(this.color);
			graph.drawLine((int)((centerX - direction.dx() * hitbox.width / 2) * zoom), (int)((centerY - direction.dy() * hitbox.height / 2) * zoom),
					(int)((centerX + direction.dx() * hitbox.width / 2) * zoom), (int)((centerY + direction.dy() * hitbox.height / 2) * zoom));
		}
	}

	public void move(List<Wall> walls, Vector2D gravity) {
		Objects.requireNonNull(walls);
		Objects.requireNonNull(gravity);

		var hitbox = this.hitbox();
		/* Are these affected by gravity? */
		//hitbox.x += gravity.dx();
		//hitbox.y += gravity.dy();

		moveTo((int)(hitbox.x + direction.dx() * SWORDSPEED), (int)(hitbox.y + direction.dy() * SWORDSPEED));
	}
	
	@Override
	public void handleCollision(Entity e) {
		Objects.requireNonNull(e);
		if(this == e || !this.intersects(e)) {
			return;
		}
		if(e instanceof Sword) {
			/* Swords cancel out upon collision. */
			//this.kill();
			//e.kill();
		}
		else if (e instanceof Wall) {
			/* Already done. */
			e.handleCollision(this);
		}
		else if (e instanceof Player) {
			/* When a player collides with a sword, they both die, unless the sword belongs to the player. */
			if(owner != e) {
				this.kill();
				e.kill();
			}
		}
		else {
			throw new IllegalArgumentException("Unknown entity subtype");
		}
	}
	
	@Override
	public void kill() {
		super.kill();
		this.owner.addSword();
	}
}
