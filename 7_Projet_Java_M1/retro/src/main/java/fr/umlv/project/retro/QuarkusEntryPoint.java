package fr.umlv.project.retro;

import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import javax.ws.rs.Consumes;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;

import org.objectweb.asm.Opcodes;

import fr.umlv.project.feature.ClassFeatureAnalyzer;
import fr.umlv.project.parser.ExtractUtil;
import fr.umlv.project.parser.OptionParser;

@javax.ws.rs.Path("/retro")
@Produces(MediaType.APPLICATION_JSON)
@Consumes(MediaType.APPLICATION_JSON)
public class QuarkusEntryPoint {

	private static final int API_VERSION = Opcodes.ASM7;
	private String[] argsTab;

	public static class OptionWeb {
		String args;
		
	    public String getArgs() {
	        return args;
	    }
	    public void setArgs(String info) {
	        this.args = info;
	    }
	}
	

	@GET
	public List<String> getMessagesFeatures() {
		Map<String, byte[]> classMap;
		ClassFeatureAnalyzer cfa;
		
		List<String> msg = new ArrayList<String>();
		List<String> options = OptionParser.getOptions(argsTab);

		var setClasses = OptionParser.getClassFileNames(argsTab, options);
		var setJars = OptionParser.getJarFileNames(argsTab, options);
		
		for (var jarName : setJars) {
			classMap = ExtractUtil.extractFileToMap(Path.of(jarName));
			cfa = ClassFeatureAnalyzer.classFeatureAnalyzerFactory(API_VERSION, classMap);
			msg.addAll(cfa.getMessageFeature());
		}
		if (setClasses.size() != 0) {
			classMap = ExtractUtil.extractAllClassesFromFilesToMap(setClasses);
			cfa = ClassFeatureAnalyzer.classFeatureAnalyzerFactory(API_VERSION, classMap);
			msg.addAll(cfa.getMessageFeature());
		}
		return msg;
	}

	
	@POST
	public void run(OptionWeb arg) {
		argsTab = arg.args.split(" ");
		Main.main(argsTab);
	}

}