package fr.umlv.roto;

import java.awt.Color;
import java.awt.Graphics2D;
import java.awt.Rectangle;
import java.util.List;
import java.util.Objects;


public class Player extends AbstractEntity implements Moveable {
	private static final int MAX_SWORDS = 10;
	private static final int MOVESPEED = 1;
	private static final int JUMPSPEED = 1;
	private static final int MAX_JUMP_DURATION = 10;
	private static final int THROW_COOLDOWN = 10;

	private final int originalX;
	private final int originalY;
	private static final int MAX_HEALTH = 3;
	private int currentHealth;
	
	private int swords;
	private Vector2D direction;
	private final Color color;
	private boolean airborne;
	private boolean moving;
	private boolean jumping;
	private int jumpDuration;
	private int swordCooldown;
	private Vector2D eyebrowsOffset;
	

	public Player(int maxSwords, Rectangle hitbox, Color color) {
		super(Objects.requireNonNull(hitbox));
		if(maxSwords <= 0) {
			throw new IllegalArgumentException("Max swords must be at least 1");
		}

		this.originalX = hitbox.x;
		this.originalY = hitbox.y;
		this.currentHealth = MAX_HEALTH;
		this.swords = maxSwords;
		this.direction = new Vector2D(0, -2);
		this.color = Objects.requireNonNull(color);
		this.airborne = true;
		this.moving = false;
		this.jumping = false;
		this.jumpDuration = 0;
		this.swordCooldown = 0;
		this.eyebrowsOffset = new Vector2D(0, hitbox.height / 4);
	}

	@Override
	public void draw(Graphics2D graph, double zoom) {
		Objects.requireNonNull(graph);
		if (zoom < 1) {
			throw new IllegalArgumentException("Invalid zoom level");
		}

		if (this.isAlive()) {
			var hitbox = this.hitbox();

			graph.setColor(color);
			graph.drawOval((int)(hitbox.x * zoom), (int)(hitbox.y * zoom), (int)(hitbox.width * zoom), (int)(hitbox.height * zoom));
			graph.fillOval((int)(hitbox.x * zoom), (int)(hitbox.y * zoom), (int)(hitbox.width * zoom), (int)(hitbox.height * zoom));
			
			/* Show the number of swords above the player's head. */
			graph.drawString("" + this.swords, (int)(hitbox.x * zoom), (int)((hitbox.y - 5) * zoom));
			
			/* Eyebrows */
			graph.setColor(Color.WHITE);
			graph.drawLine(
					(int)((hitbox.x + hitbox.width / 2 + eyebrowsOffset.dx()) * zoom),
					(int)((hitbox.y + hitbox.height / 2 + eyebrowsOffset.dy()) * zoom),
					(int)((hitbox.x + hitbox.width / 2 + eyebrowsOffset.dx() + direction.dx() * 3) * zoom),
					(int)((hitbox.y + hitbox.height / 2 + eyebrowsOffset.dy() + direction.dy() * 3) * zoom));

		}
	}

	public void setAirborne(boolean bool) {
		this.airborne = bool;
		if (!bool) {
			this.jumping = false;
			this.jumpDuration = 0;
		}
	}

	public boolean isAirborne() {
		return airborne;
	}

	public void setDirection(Vector2D direction) {
		this.direction = Objects.requireNonNull(direction);
		this.moving = true;
	}

	public void jump() {
		/* Only groundborne players can jump. */
		if (!this.airborne) {
			this.airborne = true;
			this.jumping = true;
			this.jumpDuration = 0;
		}
	}

	public void move(List<Wall> walls, Vector2D gravity) {
		Objects.requireNonNull(walls);
		Objects.requireNonNull(gravity);

		var hitbox = this.hitbox();
		var oldX = hitbox.x;
		var oldY = hitbox.y;

		var newX = oldX;
		var newY = oldY;
		
		var triggerGravity = true;
		
		if(swordCooldown > 0) {
			swordCooldown--;
		}

		if (moving) {
			newX += direction.dx() * MOVESPEED;
			newY += direction.dy() * MOVESPEED;
			this.moving = false;
		}
		if (jumping && jumpDuration < MAX_JUMP_DURATION) {
			newX -= gravity.dx() * JUMPSPEED;
			newY -= gravity.dy() * JUMPSPEED;
			this.jumpDuration++;
			triggerGravity = false;
		}
		
		eyebrowsOffset = new Vector2D(-gravity.dx() * hitbox.width / 4, -gravity.dy() * hitbox.height / 4);

		moveTo(newX, newY);
		for (var wall : walls) {
			if (this.intersects(wall)) {
				moveTo(oldX, oldY);
				newX = oldX;
				newY = oldY;
			}
		}

		if(triggerGravity) {
			this.fall(newX, newY, gravity, walls);
		}
	}
	
	private void fall(int oldX, int oldY, Vector2D gravity, List<Wall> walls) {
		int newX = (int)(oldX + gravity.dx());
		int newY = (int)(oldY + gravity.dy());

		moveTo(newX, newY);
		for (var wall : walls) {
			if (this.intersects(wall)) {
				this.jumping = false;
				this.airborne = false;
				moveTo(oldX, oldY);
				return;
			}
		}
	}

	public Sword throwSword() {
		if (this.isAlive() && swords > 0 && swordCooldown == 0) {
			var r = hitbox();
			//r.x += r.width * direction.dx();
			//r.y += r.height * direction.dy();
			swords--;
			swordCooldown = THROW_COOLDOWN;
			return new Sword(r, new Vector2D(direction), this.color, this);
		}
		return null;
	}

	public void addSword() {
		this.swords++;
	}

	@Override
	public void handleCollision(Entity e) {
		Objects.requireNonNull(e);
		if(this == e || !this.intersects(e)) {
			return;
		}
		if (e instanceof Player) {
			/* When two players collide, they die if their opponent has at least one spare sword.
			 * Otherwise, they can cross each other. */
			Player p = (Player) e;
			if (this.swords > 0) {
				p.kill();
			}
			if (p.swords > 0) {
				this.kill();
			}
		} else if (e instanceof Sword) {
			/* When a player collides with a sword, they both die. */
			//this.kill();
			//e.kill();
			e.handleCollision(this);
		} else if (e instanceof Wall) {
			/* This should not happen since the move() method prevents it. */
			// throw new IllegalStateException("Unexpected Player / Wall collision!");
		} else {
			throw new IllegalArgumentException("Unknown entity subtype");
		}
	}
	
	@Override
	public void kill() {
		moveTo(originalX, originalY);
		this.airborne = true;
		this.jumping = false;
		this.jumpDuration = 0;
		this.currentHealth--;
		if(currentHealth <= 0) {
			super.kill();
		}
	}

}
