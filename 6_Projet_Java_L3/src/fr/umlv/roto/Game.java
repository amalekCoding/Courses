package fr.umlv.roto;

import java.awt.Color;
import fr.umlv.zen5.Event.Action;
import fr.umlv.zen5.KeyboardKey;

import java.awt.Rectangle;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Locale;
import java.util.Objects;

import fr.umlv.zen5.Application;
import fr.umlv.zen5.ApplicationContext;

public class Game {

	private static final int MIN_WALL_SIZE = 16;
	private final ArrayList<Wall> walls;
	private final ArrayList<Moveable> entities;
	private final HashMap<KeyboardKey, Runnable> keyMap;
	private final HashSet<KeyboardKey> activeKeys;
	private Vector2D gravity;
	private int zoomLevel;
	private boolean keepGoing;
	private final int phase;
	private int highestZoom;
	private final Runnable changeGravityFunction;
	private final int swordsPerPlayer;

	private enum GravityType {
		GRAVITY_DOWN, GRAVITY_UPDOWN, GRAVITY_UDLR, GRAVITY_360;
		private static GravityType fromString(String s) {
			switch(s) {
			case "1":
				return GRAVITY_DOWN;
			case "2":
				return GRAVITY_UPDOWN;
			case "4":
				return GRAVITY_UDLR;
			case "360":
				return GRAVITY_360;
			default:
				throw new IllegalArgumentException("Unknown gravity type");
			}
		}
	}

	private Game(GravityType type, int phase, int swordsPerPlayer) {
		this.walls = new ArrayList<>();
		this.entities = new ArrayList<>();
		this.keyMap = new HashMap<>();
		this.activeKeys = new HashSet<>();
		this.gravity = new Vector2D(0, 2);
		this.zoomLevel = 10;
		this.keepGoing = true;
		this.phase = phase;
		this.highestZoom = 10;
		switch(type) {
		case GRAVITY_DOWN:
			changeGravityFunction = () -> {};
			break;
		case GRAVITY_UPDOWN:
			changeGravityFunction = () -> gravity = new Vector2D(gravity, Math.toRadians(180));
			break;
		case GRAVITY_UDLR:
			changeGravityFunction = () -> gravity = new Vector2D(gravity, Math.toRadians(90));
			break;
		default: // case GRAVITY_360:
			changeGravityFunction = () -> gravity = new Vector2D(gravity, Math.toRadians(20));
			break;
		}
		this.swordsPerPlayer = swordsPerPlayer;
	}

	/**
	 * Updates the highest zoom value. By default, zooming is not possible until this method is called.
	 * @param windowWidth the screen's width
	 * @param windowHeight the screen's height
	 */
	public void computeHighestZoom(int windowWidth, int windowHeight) {
		var firstWallX = walls.get(0).hitbox().x;
		var lastWallX = walls.get(walls.size() - 1).hitbox().x + MIN_WALL_SIZE;
		var firstWallY = walls.get(0).hitbox().y;
		var lastWallY = walls.get(walls.size() - 1).hitbox().y + MIN_WALL_SIZE;

		var highestZoomX = (int) ((float) windowWidth / (lastWallX - firstWallX) * 10);
		var highestZoomY = (int) ((float) windowHeight / (lastWallY - firstWallY) * 10);

		if(highestZoomX * (lastWallY - firstWallY) > windowHeight * 10) {
			this.highestZoom = highestZoomY;
		} else {
			this.highestZoom = highestZoomX;
		}
	}

	private boolean verifyBoard() {

		var firstXwall = walls.get(0).hitbox().x;
		var firstYwall = walls.get(0).hitbox().y;

		var lastXwall = walls.get(walls.size() - 1).hitbox().x;
		var lastYwall = walls.get(walls.size() - 1).hitbox().y;

		var nbCols = (lastXwall / MIN_WALL_SIZE) - (firstXwall / MIN_WALL_SIZE);
		var nbLines = (lastYwall / MIN_WALL_SIZE) - (firstYwall / MIN_WALL_SIZE);

		for (var i = 0; i <= nbCols; i++) {
			/* Verify the first line */
			if (firstYwall != walls.get(i).hitbox().y) {
				return false;
			}
			/* Verify the last line */
			if (lastYwall != walls.get(walls.size() - 1 - i).hitbox().y) {
				return false;
			}
		}

		int i = nbCols + 1; // The 2nd line index in the array walls.
		var nbColsPerLine = 0;
		for (var line = (firstYwall / MIN_WALL_SIZE) + 1; line < nbLines; line++) {
			/* Find the index of the last wall on the line and count the number of walls per line */
			for (; i < walls.size(); i++) {
				if (walls.get(i).hitbox().y / 16 != line) {
					break;
				}
				nbColsPerLine++;
			}
			/* Verify the first column */
			if (firstXwall != walls.get(i - nbColsPerLine).hitbox().x) {
				return false;
			}
			/* Verify the last column */
			if (lastXwall != walls.get(i - 1).hitbox().x) {
				return false;
			}
			nbColsPerLine = 0;
		}
		return true;
	}

	private void loadLevel(String path) throws FileNotFoundException, IOException {
		Objects.requireNonNull(path);

		var file = new FileReader(path);
		int currentChar;
		int nbLines = 0;
		int nbCols = 0;

		while ((currentChar = file.read()) != -1) {
			char c = (char) currentChar;

			switch (c) {
			case '\r':
				continue;
			case '\n':
				nbCols = 0;
				nbLines++;
				continue;

			case 'W':
				walls.add(new Wall(
						new Rectangle(nbCols * MIN_WALL_SIZE, nbLines * MIN_WALL_SIZE, MIN_WALL_SIZE, MIN_WALL_SIZE)));
				break;

			case 'O':
				var p1 = new Player(swordsPerPlayer,
						new Rectangle(nbCols * MIN_WALL_SIZE, nbLines * MIN_WALL_SIZE, MIN_WALL_SIZE, MIN_WALL_SIZE),
						Color.RED);
				entities.add(p1);
				keyMap.put(KeyboardKey.Q,     () -> p1.setDirection(new Vector2D(gravity, Math.toRadians(+90))));
				keyMap.put(KeyboardKey.D,     () -> p1.setDirection(new Vector2D(gravity, Math.toRadians(-90))));
				keyMap.put(KeyboardKey.Z,     p1::jump);
				keyMap.put(KeyboardKey.SPACE, () -> {
					var sword = p1.throwSword();
					if(sword != null) {
						entities.add(sword);
					}
				});
				break;

			case 'T':
				var p2 = new Player(swordsPerPlayer,
						new Rectangle(nbCols * MIN_WALL_SIZE, nbLines * MIN_WALL_SIZE, MIN_WALL_SIZE, MIN_WALL_SIZE),
						Color.BLUE);
				entities.add(p2);
				keyMap.put(KeyboardKey.LEFT,  () -> p2.setDirection(new Vector2D(gravity, Math.toRadians(+90))));
				keyMap.put(KeyboardKey.RIGHT, () -> p2.setDirection(new Vector2D(gravity, Math.toRadians(-90))));
				keyMap.put(KeyboardKey.UP,    p2::jump);
				keyMap.put(KeyboardKey.CTRL,  () -> {
					var sword = p2.throwSword();
					if(sword != null) {
						entities.add(sword);
					}
				});
				break;

			case ' ':
				break;

			default:
				throw new IOException("Illegal character encountered");
			}
			nbCols++;
		}

		file.close();
		if(!verifyBoard()) {
			throw new IOException("Invalid level format");
		}
	}

	/**
	 * Move every object, updating their position. Collision detection happens here.
	 */
	public void moveObjects() {
		entities.forEach(entity -> {
			entity.move(walls, gravity);
			walls.forEach(wall -> {
				entity.handleCollision(wall);
			});
			entities.forEach(target -> {
				entity.handleCollision(target);
			});
		});
		
		entities.removeIf(entity -> !entity.isAlive());
		walls.removeIf(wall -> !wall.isAlive());
		
	}

	/**
	 * Checks if any key is currently pressed, and trigger its associated action.
	 * @param context The application's context
	 */
	public void handleInput(ApplicationContext context) {
		Objects.requireNonNull(context);

		var ev = context.pollOrWaitEvent(20);
		if (ev != null) {
			var action = ev.getAction();
			if (action == Action.KEY_PRESSED) {
				var key = ev.getKey();
				activeKeys.add(key);
			} else if (action == Action.KEY_RELEASED) {
				var key = ev.getKey();
				activeKeys.remove(key);
			}
		}
		
		/* For each active key, trigger its associated lambda. */
		activeKeys.forEach(key -> {
			var keyAction = keyMap.get(key);
			if(keyAction != null) {
				keyAction.run();
			}
		});
	}
	
	/**
	 * Zoom out one step, if possible.
	 */
	public void zoomOut() {
		if(this.zoomLevel > 10) {
			this.zoomLevel-= 1;
		}
	}

	/**
	 * Zoom in one step, if possible.
	 */
	public void zoomIn() {
		if(this.zoomLevel < this.highestZoom) {
			this.zoomLevel += 1;
		}
	}
	
	/**
	 * Triggers a change of the gravitational field. Exact behaviour is selected at game creation.
	 */
	public void changeGravity() {
		changeGravityFunction.run();
	}

	/**
	 * Creates a game object with options given as parameter.
	 * @param args An array of arguments, stored as strings.
	 * Valid arguments are:
	 * --level NAME (default-level)
	 * --gravity TYPE (1)
	 * --gravity-phase PHASE (10 000)
	 * --swords SWORDS (20)
	 * @return A new game object whose behaviour matches the selected options.
	 * @throws IOException
	 * @throws FileNotFoundException
	 */
	public static Game parseArguments(String[] args) throws IOException, FileNotFoundException {
		Objects.requireNonNull(args);

		String level = "default-level.txt";
		GravityType gravityType = GravityType.GRAVITY_DOWN;
		int phase = 10_000;
		int swordsPerPlayer = 20;

		for(int i = 0; i < args.length; ++i) {
			switch(args[i]) {
			case "--level":
				i++;
				if(i < args.length) {
					level = args[i];
				} else {
					throw new IllegalArgumentException("Missing level name");
				}
				break;
			case "--gravity":
				i++;
				if(i < args.length) {
					gravityType = GravityType.fromString(args[i]);
				} else {
					throw new IllegalArgumentException("Missing gravity type");
				}
				break;
			case "--gravity-phase":
				i++;
				if(i < args.length) {
					phase = Integer.parseInt(args[i]);
				} else {
					throw new IllegalArgumentException("Missing gravity phase");
				}
				break;
			case "--swords":
				i++;
				if(i < args.length) {
					swordsPerPlayer = Integer.parseInt(args[i]);
				} else {
					throw new IllegalArgumentException("Missing swords per player amount");
				}
				break;
			default:
				throw new IllegalArgumentException("Unknown argument");
			}
		}
		
		if(phase <= 0) {
			throw new IllegalArgumentException("Gravity phase must be at least 1");
		}
		if(swordsPerPlayer <= 0) {
			throw new IllegalArgumentException("Swords per player must be at least 1");
		}
		
		var game = new Game(gravityType, phase, swordsPerPlayer);
		game.loadLevel(level);
		game.keyMap.put(KeyboardKey.M,   game::zoomOut);
		game.keyMap.put(KeyboardKey.P,   game::zoomIn);
		game.keyMap.put(KeyboardKey.ALT, () -> game.keepGoing = false);
		
		return game;
	}

	
	public static void main(String[] args) {

		Game game;

		try {
			game = Game.parseArguments(args);
		} catch (FileNotFoundException e) {
			System.out.println(e.getMessage());
			return;
		} catch (IOException e) {
			System.out.println(e.getMessage());
			return;
		}

		Application.run(Color.GRAY, context -> {
			var screenInfo = context.getScreenInfo();
			var screenWidth = (int) screenInfo.getWidth();
			var screenHeight = (int) screenInfo.getHeight();
			game.computeHighestZoom(screenWidth, screenHeight);
			
			long firstTime = System.currentTimeMillis();
			long secondTime;

			while (game.keepGoing) {

				secondTime = System.currentTimeMillis();
				if((secondTime - firstTime) >= game.phase) {
					game.changeGravity();
					firstTime = secondTime;
				}

				/* Gather user input. */
				game.handleInput(context);

				/* Move objets according to trajectory and gravity. */
				game.moveObjects();

				/* Display the updated scene. */
				context.renderFrame(graph -> {
					graph.clearRect(0, 0, screenWidth, screenHeight);

					game.walls.forEach(wall -> {
						wall.draw(graph, game.zoomLevel / 10.0);
					});

					game.entities.forEach(player -> {
						player.draw(graph, game.zoomLevel / 10.0);
					});

				});
			}

			context.exit(0);
		});
	}
}
