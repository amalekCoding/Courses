package fr.umlv.project.parser;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FilenameFilter;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Objects;
import java.util.Set;
import java.util.jar.JarFile;
import java.util.stream.Collectors;

import org.objectweb.asm.ClassReader;



public class ExtractUtil {
	
	/** Private constructor : Contains only static methods. */
	private ExtractUtil() {
	}



	/**
	 * All Java classes start with a magic word called : CAFE BABE which is a
	 * hexadecimal number.
	 */
	private static boolean isClassFile(byte[] bytes) {
		return bytes.length >= 4 && bytes[0] == ((byte) 0xCA) && bytes[1] == ((byte) 0xFE) && bytes[2] == ((byte) 0xBA)
				&& bytes[3] == ((byte) 0xBE);
	}

	/**
	 * From an input path extract all the names of the classes and it's bytes
	 * contents into a map whether it's just a .class file or .jar file or a
	 * directory that contains both.
	 * 
	 * @param inputPath
	 * @return A map that contains the classes names associated to it's bytes
	 *         contents.
	 */
	public static Map<String, byte[]> extractFileToMap(Path inputPath) {
		Objects.requireNonNull(inputPath);
		String inputName = inputPath.toString();

		if (inputName.endsWith(".class")) {
			return extractClassFromFileToMap(inputName);
		} else if (inputName.endsWith(".jar")) {
			return extractClassesFromJarToMap(inputPath);
		} else {
			throw new IllegalArgumentException("File must be a .class or .jar");
		}
	}
	
	
	public static Map<String, byte[]> extractAllClassesFromFilesToMap(Set<String> setInput) {
		HashMap<String, byte[]> classMap = new HashMap<>();
		ClassReader cr;
		
		for(var input : setInput) {
			byte[] bytesContent = transformFileToByte(input);
			checkIfIsAGoodClassFile(input, bytesContent);
			cr = new ClassReader(bytesContent);
			classMap.put(cr.getClassName()+".class", bytesContent);
		}
		
		return classMap;
	}
	

	private static boolean checkIfIsAGoodClassFile(String input, byte[] bytesContent) {
		if (!input.endsWith(".class") || !isClassFile(bytesContent)) {
			throw new IllegalStateException("The file " + input + " is not a Java .class file.");
		}
		return true;
	}

	/**
	 * Put in a {@code Map<String, byte[]>} the name of the file of the class and
	 * it's content in a byte array.
	 **/
	private static Map<String, byte[]> extractClassFromFileToMap(String input) {
		HashMap<String, byte[]> classMap = new HashMap<>();

		byte[] bytesContent = transformFileToByte(input);
		checkIfIsAGoodClassFile(input, bytesContent);
		ClassReader cr = new ClassReader(bytesContent);
		classMap.put(cr.getClassName() + ".class", bytesContent);
		return classMap;
	}

	private static Map<String, byte[]> extractClassesFromJarToMap(Path input) {
		Map<String, byte[]> classNameMap = new HashMap<>();
		try (JarFile jarInput = new JarFile(input.toFile())) {
			jarInput.stream().forEach(entry -> {
				try (InputStream inputStream = jarInput.getInputStream(entry)) {
					if (entry.getName().endsWith(".class") && !entry.getName().contains("org/objectweb/asm")) {
						byte[] bytesContent = getBytesContentAndCheckGoodClassFile(inputStream, entry.getName(), input);
						classNameMap.put(entry.getName(), bytesContent);
					}
				} catch (IOException e) {
					throw new IllegalStateException("Failed to extract the classes from Jar file : " + input, e);
				}
			});
		} catch (IOException e) {
			throw new IllegalStateException("Failed to extract the classes from Jar file. " + e);
		}
		return classNameMap;
	}
	
	
	private static byte[] getBytesContentAndCheckGoodClassFile(InputStream inputStream, String name, Path input) throws IOException {
		byte[] bytesContent = toByteArray(inputStream);
		if (!isClassFile(bytesContent)) {
			throw new IllegalStateException(
					"The file " + name + " in " + input + " is not a Java .class file.");
		}
		return bytesContent;
	}
	
	
	
	

	public static byte[] transformFileToByte(String input) {
		try (FileInputStream inputStream = new FileInputStream(input)) {
			return toByteArray(inputStream);
		} catch (IOException e) {
			throw new IllegalArgumentException("Couldn't find the " + input + " file.\n" + e);
		}

	}

	/**
	 * Take an inputStream as argument and return a byte array with it's content.
	 * 
	 * @param inputStream
	 * @return A byte array filled with the content of the input stream given.
	 * @throws IOException
	 */
	private static byte[] toByteArray(InputStream inputStream) throws IOException {
		ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();
		copy(inputStream, byteArrayOutputStream);
		return byteArrayOutputStream.toByteArray();
	}

	/**
	 * Copy the contents of an InputStream to an OutputStream given as arguments.
	 * 
	 * @param inputStream
	 * @param outputStream
	 * @throws IOException
	 */
	public static void copy(InputStream inputStream, OutputStream outputStream) throws IOException {
		int read;
		while ((read = inputStream.read()) != -1) {
			outputStream.write(read);
		}
		inputStream.close();

	}


	// test if the file exists
	private static boolean isValidFile(String file) {
		var tmp = new File(file);
		if (!tmp.isFile()) {
			throw new IllegalArgumentException("\n"+file+" : file or folder does not exist !");
		}
		return file.endsWith(".class") || file.endsWith(".jar");
	}
	
	// list files of folder
	private static void walkFolder(Set<String> res, String folder) {
		File f = new File(folder);
		FilenameFilter filter = new FilenameFilter() {
			@Override
			public boolean accept(File f, String name) {
				return name.endsWith(".class") || name.endsWith(".jar");
			}
		};
		res.addAll(Arrays.stream(f.listFiles(filter)).map(File::toString).collect(Collectors.toList()));
	}

	public static Set<String> parse(List<String> args) {
		Objects.requireNonNull(args);
		Set<String> res = new HashSet<>();
		for (String arg : args) {
			if (Files.isDirectory(Paths.get(arg))) {
				walkFolder(res, arg);
			}
			else if (isValidFile(arg)) {
				res.add(arg);
			}
		}
		return res;
	}
	

}
