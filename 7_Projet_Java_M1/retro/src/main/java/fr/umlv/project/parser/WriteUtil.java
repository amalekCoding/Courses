package fr.umlv.project.parser;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Objects;
import java.util.jar.JarEntry;
import java.util.jar.JarFile;
import java.util.jar.JarOutputStream;

import org.objectweb.asm.ClassWriter;

public class WriteUtil {

	
	
	public static void rewrite(int api, Map<String, byte[]> classMap, Path input, Path output) {
		Objects.requireNonNull(input);
		String inputName = input.toString();

		if (inputName.endsWith(".class")) {
			writeNewClasses(classMap);
		} else if (inputName.endsWith(".jar")) {
			writeNewJar(api, classMap, input, output);
		} else {
			throw new IllegalArgumentException("File must be a .class or .jar");
		}
	}
	
	
	public static void deleteDirectory(String emplacement) {
		File path = new File(emplacement);
		if (path.exists()) {
			File[] files = path.listFiles();
			for (int i = 0; i < files.length; i++) {
				if (files[i].isDirectory()) {
					deleteDirectory(path + "\\" + files[i]);
				}
				files[i].delete();
			}
		}
	}

	public static void writeNewClasses(Map<String, byte[]> classNameMap) {
		classNameMap.forEach((name, bytesContent) -> {
			try (FileOutputStream out = new FileOutputStream(name, false)) {
				out.write(bytesContent);
			} catch (IOException e) {
				throw new IllegalStateException(e);
			}
		});
	}

	public static void writeNewJar(int version, Map<String, byte[]> classNameMap, Path input, Path output) {
		ArrayList<String> classNameAdded = new ArrayList<>();

		try (JarFile jarInput = new JarFile(input.toFile());
				JarOutputStream outputStream = new JarOutputStream(Files.newOutputStream(output))) {
			rewriteJarWithUpdatedClasses(jarInput, outputStream, classNameMap, classNameAdded);
			writeFilesIfGenerated(classNameMap, classNameAdded, outputStream);
		} catch (IOException e) {
			throw new IllegalStateException("Failed to create the new Jar file. ", e);
		}
	}
	
	private static void rewriteJarWithUpdatedClasses(JarFile jarInput, JarOutputStream outputStream, Map<String, byte[]> classNameMap, List<String> classNameAdded) {
		jarInput.stream().forEach(entry -> {
			try (InputStream inputStream = jarInput.getInputStream(entry)) {
				outputStream.putNextEntry(new JarEntry(entry.getName()));
				if (entry.getName().endsWith(".class") && classNameMap.containsKey(entry.getName())) {
					outputStream.write(classNameMap.get(entry.getName()));
					classNameAdded.add(entry.getName());
				} else {
					ExtractUtil.copy(inputStream, outputStream);
				}
			} catch (IOException e) {
				throw new IllegalStateException("Failed to create the new Jar file. ", e);
			}
		});
	}
	
	
	private static void writeFilesIfGenerated(Map<String, byte[]> classNameMap, List<String> classNameAdded, JarOutputStream outputStream) {
		classNameMap.forEach((className, bytesContent) -> {
			if (!classNameAdded.contains(className)) {
				try {
					outputStream.putNextEntry(new JarEntry(className));
					outputStream.write(bytesContent);
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		});
	}
	
	
	
	public static void writeByteToFile(String path, byte[] data) throws IOException {
		Objects.requireNonNull(path); Objects.requireNonNull(data);
		path = path.contains(".class") ? path : path +".class";
		File directory = new File(path.substring(0, path.lastIndexOf("/")));
		directory.mkdirs();
		try (FileOutputStream out = new FileOutputStream(path)) {
			out.write(data);
		}
	}
	
	/** Create a class writer and a new file with the same same. */
	public static ClassWriter createClassWriterAndNewFile(String fileName) {
		ClassWriter writer = new ClassWriter(ClassWriter.COMPUTE_FRAMES);	
		try {
			writeByteToFile(fileName, writer.toByteArray());
		} catch (IOException e) {
			e.printStackTrace();
		}
		return writer;
	}
	

}
