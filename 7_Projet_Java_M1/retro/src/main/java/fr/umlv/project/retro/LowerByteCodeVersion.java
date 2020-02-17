package fr.umlv.project.retro;

import org.objectweb.asm.ClassVisitor;


/**
 * Class that allow you to lower the Java byte code version by visiting a class.
 */
public class LowerByteCodeVersion extends ClassVisitor {

	/** The new version that we want it to be.  */
	private final int targetVersion;

	
	public LowerByteCodeVersion(int api, ClassVisitor classVisitor, int targetVersion) {
		super(api, classVisitor);
		this.targetVersion = targetVersion;
	}

	@Override
	public void visit(int version, int access, String name, String signature, String superName, String[] interfaces) {
		if (version > targetVersion) {
			version = targetVersion;
		}
		super.visit(targetVersion, access, name, signature, superName, interfaces);
	}

}
