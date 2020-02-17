package fr.umlv.project.feature.tryWithResourcesFeature;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Objects;
import java.util.Set;

import org.objectweb.asm.ClassReader;
import org.objectweb.asm.ClassVisitor;
import org.objectweb.asm.Label;
import org.objectweb.asm.MethodVisitor;
import org.objectweb.asm.Opcodes;

import fr.umlv.project.feature.FeatureCollector;

public class CollectClassesWithTryWithRes implements FeatureCollector {

	private final Collection<byte[]> bytesContents;
	private final Map<String, List<TryWithResInfo>> tryWithResourcesClassesInfo = new HashMap<>();

	private final int api;

	private CollectClassesWithTryWithRes(int api, Collection<byte[]> bytesContents) {
		this.bytesContents = Collections.unmodifiableCollection(bytesContents);
		this.api = api;
	}

	public static CollectClassesWithTryWithRes collectClassesWithTryWithResFactory(int api,
			Collection<byte[]> bytesContents) {
		var tryWithResCollector = new CollectClassesWithTryWithRes(api, Objects.requireNonNull(bytesContents));
		tryWithResCollector.collect();
		return tryWithResCollector;
	}

	private void collect() {
		bytesContents.forEach(byteContent -> {
			ClassReader cr = new ClassReader(byteContent);
			cr.accept(new DetectTryWithResource(api), 0);
		});
	}

	@Override
	public void printFeature() {
		tryWithResourcesClassesInfo.values().forEach(lstTwrInfos -> 
		lstTwrInfos.forEach(System.out::println));
	}
	
	@Override
	public List<String> getFeaturesMessage() {
		List<String> lst = new ArrayList<>();
		tryWithResourcesClassesInfo.values().forEach(lstTwrInfos -> 
		lstTwrInfos.forEach(twrInfo -> lst.add(twrInfo.toString()))
		);
		return lst;
	}
	

	@Override
	public Set<String> getClassesNamesFeature() {
		return new HashSet<>(tryWithResourcesClassesInfo.keySet());
	}

	private class DetectTryWithResource extends ClassVisitor {

		private DetectTryWithResource(int api) {
			super(api);
		}

		/** The name of the current class visited. */
		private String classOwner;
		private String methodOwnerName;
		private String methodOwnerDesc;
		private List<TryWithResInfo> twrInfos = new ArrayList<>();

		@Override
		public void visit(int version, int access, String name, String signature, String superName,
				String[] interfaces) {
			this.classOwner = name;
		}

		@Override
		public MethodVisitor visitMethod(int access, String name, String desc, String signature, String[] exceptions) {
			this.methodOwnerName = name;
			this.methodOwnerDesc = desc;
			MethodVisitor mv = super.visitMethod(access, name, desc, signature, exceptions);
			return new MethodVisitor(api, mv) {

				private int line;
				private String owner = "";
				private boolean closePresent = false;

				@Override
				public void visitLineNumber(int line, Label start) {
					this.line = line;
					super.visitLineNumber(line, start);
				}

				@Override
				public void visitMethodInsn(int opcode, String owner, String name, String desc, boolean itf) {
					if (opcode == Opcodes.INVOKEVIRTUAL && owner.equals("java/lang/Throwable")
							&& name.equals("addSuppressed") && desc.equals("(Ljava/lang/Throwable;)V")
							&& closePresent) {
						twrInfos.add(
								new TryWithResInfo(classOwner, methodOwnerName, methodOwnerDesc, this.owner, line));
						closePresent = false;
					} else if (name.equals("close") && desc.equals("()V")) {
						closePresent = true;
						this.owner = owner;
					}
					super.visitMethodInsn(opcode, owner, name, desc, itf);
				}
			};

		}

		@Override
		public void visitEnd() {
			if (twrInfos.size() != 0) {
				tryWithResourcesClassesInfo.put(classOwner, twrInfos);
			}
			super.visitEnd();
		}

	}

}
