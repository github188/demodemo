/**
* $ Name LastChangeRevision LastChangeDate LastChangeBy $
* $Id$
*/

package com.nsn.reporting.chart;

import static com.nsn.reporting.data.ChartInfomation.HEADER_QUALITY;
import static com.nsn.reporting.data.ChartInfomation.HEADER_SIZE;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.HashMap;
import java.util.Map;

import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.CommandLineParser;
import org.apache.commons.cli.HelpFormatter;
import org.apache.commons.cli.Option;
import org.apache.commons.cli.Options;
import org.apache.commons.cli.ParseException;
import org.apache.commons.cli.PosixParser;
import org.jfree.chart.ChartUtilities;

import com.nsn.reporting.data.ChartInfomation;
import com.nsn.reporting.data.PieChartInfomation;
import com.nsn.reporting.data.StackedChartInfomation;
import com.nsn.reporting.data.StackedLineChartInfomation;

public class ChartReport {
	
	public static final String LOGLEVEL = "loglevel";
	public static final String QUALITY = "quality";
	public static final String FORMAT = "format";
	public static final String STDIN = "stdin";
	public static final String VERSION = "version";
	public static final String ROOT = "root";
	public static final String OUTPUT = "output";
	public static final String VARIABLE = "var";
	public static final String DATEXT = "data_suffix";
	public static final String OUTEXT = "out_suffix";
	public static final String CHARTSIZE = "size";
	
	public static final String version = "0.0.1";
	public static final String defaultSize = "600x400";
	public static final String defaultQuality = "1";
	
	private static int tempCount = 0;
	
	private CommandLine cmd = null;
	private Map<String, String> globalHeader = null;
	private File outputRoot = null;
	private File outputFile = null;
	private File workRoot = null;
	private File curData = null;
	
	private String datExt = null;
	private String outExt = null;
	
	private boolean debug = true;
	
	public static void main(String[] args) throws ParseException{
		ChartReport report = new ChartReport();
		report.run(args);
		
		report.exit(0);
	}
	
	
	public void run(String[] args){
		Options options = new Options();
		options.addOption(VERSION, false, "show version");
		options.addOption(FORMAT, true, "the format of output chart, default is JPEG.");
		options.addOption(ROOT, true, "the root path for generating reporting.");
		options.addOption(OUTPUT, true, "the filename of output chart");
		options.addOption(STDIN, false, "read the data from stdin to generate reporting");
		options.addOption(LOGLEVEL, true, "default is INFO, [DEBUG, INFO, WARN]");
		options.addOption(QUALITY, true, "the quality of output chart, default is 1.0. [0f ~ 1.0f]");
		options.addOption(VARIABLE, true, "set global parameter to all charts.");
		options.addOption(DATEXT, true, "the data file suffix");
		options.addOption(OUTEXT, true, "the output file suffix");
		options.addOption(CHARTSIZE, true, "the size of output chart, default is 600x400");
		
		options.addOption("d", false, "dummy option");
		
		options.getOption(VARIABLE).setArgs(Option.UNLIMITED_VALUES);
		
		final String usage = "ChartReport [options] <input>"; 
		
		try{
			CommandLineParser parser = new PosixParser();
			cmd = parser.parse(options, args);			
		}catch(ParseException e){
			HelpFormatter formatter = new HelpFormatter();
			formatter.printHelp(usage, options);
			System.exit(-1);
		}
		
		if(cmd.hasOption(LOGLEVEL) && 
		   cmd.getOptionValue(LOGLEVEL).equalsIgnoreCase("DEBUG")){
			this.debug = true;
		}
				
		if(cmd.hasOption(VERSION)){
			echo("ChartReport " + version);
			return;
		}
		
		if(cmd.hasOption(STDIN) || cmd.getArgs().length > 0){
			datExt = cmd.getOptionValue(DATEXT, ".dat");
			outExt = cmd.getOptionValue(OUTEXT, ".jpg");
	
			if(!initDirectory())return;
			globalHeader = initGlobalChartHeader(cmd.getOptionValues(VARIABLE));
			globalHeader.put(HEADER_SIZE, 
							 cmd.getOptionValue(CHARTSIZE, defaultSize));
			globalHeader.put(HEADER_QUALITY, 
							 cmd.getOptionValue(QUALITY, defaultQuality));
		}
		
		if(cmd.hasOption(STDIN)){
			generateReport(System.in);
		}else if(cmd.getArgs().length > 0){
			generateReport(cmd.getArgs());
		}else {
			HelpFormatter formatter = new HelpFormatter();
			formatter.printHelp(usage, options);
		}
	}
	
	public void generateReport(String[] dataSources){				
		for(String input: dataSources){
			File inputFile = new File(workRoot, input);
			if(inputFile.isDirectory()){
				this.generateReportDirectory(inputFile);
			}else {
				this.generateReport(inputFile);
			}
		}
	}
	
	public void generateReport(File data){
		if(data.isFile()) {
			curData = data;
			
			String infile = data.getAbsolutePath();
			infile = infile.replace(this.workRoot.getAbsolutePath() + File.separator, "");
			echo("input:" + infile);
			
			try {
				this.generateReport(new FileInputStream(data));
			} catch (FileNotFoundException e) {
				exception(e);
				echo("Not found file:" + data.getAbsolutePath());
			}finally{
				curData = null;
			}
		}else {
			echo("Not found file:" + data.getAbsolutePath());
		}
	}

	public void generateReportDirectory(File data){
		for(File f: data.listFiles()){
			if(!this.filter(f.getName())) continue;
			if(f.isFile() && f.getName().endsWith(datExt)){
				generateReport(f);
			}else if(f.isDirectory()){
				generateReportDirectory(f);
			}
		}
	}
	
	public void generateReport(InputStream data){
		BufferedReader input = new BufferedReader(new InputStreamReader(data));
		
		int lineCount = 0;
		try {
			ChartInfomation info = null;
			for(String line=input.readLine(); 
				line != null;
				line = input.readLine()){
				lineCount++;
				
				if(line.startsWith("#") || line.trim().equals("")) continue;
				//a single '.' for split chart data in one file.
				if(line.trim().endsWith(".")){
					this.generateReport(info);
					info = null;
					continue;
				}
				
				if(info == null){
					info = createChartInformation(line);
				}else {
					info.parseData(line, lineCount);
				}
			}
			//end of data file.
			this.generateReport(info);
		} catch (IOException e) {
			exception(e);
			echo("read input file error!");
		}
	}
	
	public void generateReport(ChartInfomation info){
		if(info == null)return;
		if(!info.isComplete()){
			echo("incomplete data:" + info.toString());
			return;
		}
		
		for(String header : this.globalHeader.keySet()){
			info.processHeader(header, globalHeader.get(header));
		}
		
		FileOutputStream os = null;
		try {
			os = new FileOutputStream(outputChartFile(info));
			String[] size = info.getSize().split("x", 2);
			int width = Integer.parseInt(size[0]);
			int height = Integer.parseInt(size[1]);
			
			ChartUtilities.writeChartAsJPEG(os, info.getQuality(), info.createChart(), width, height, null);
		}catch(Exception e){
			exception(e);
			echo("error:" + e.getMessage());
		}finally {
			try {
				os.close();
			} catch (Exception e) {}
		}
	}
	
	private boolean filter(String name){
		if(name.startsWith(".svn") || name.startsWith("_")) return false;
		return true;
	}
	
	private File outputChartFile(ChartInfomation info){
		String outfile = info.getOutput();
		if(outfile == null && curData != null) {
			outfile = curData.getAbsolutePath();
			outfile = outfile.replace(workRoot.getAbsolutePath() + File.separator, "");
			outfile = outfile.replaceFirst(datExt + "$", outExt);
		}else if(outfile == null) {
			if(this.outputFile.exists()){
				String[] f = outputFile.getName().split("\\.", 2);
				for(File out = this.outputFile; out.exists();){
					outfile = String.format("%s%03d.%s", f[0],tempCount++, f[1]);
					out = new File(this.outputFile.getParentFile(), outfile);
				}
			}else {
				outfile = outputFile.getName();
			}
		}
		
		echo("output:" + outfile);
		File file = new File(outputRoot, outfile);
		if(!file.getParentFile().isDirectory()){
			file.mkdirs();
		}
		
		return file;
	}
	
	private Map<String, String> initGlobalChartHeader(String[] headers){
		Map<String, String> result = new HashMap<String, String>();
		if(headers != null){
			for(String header : headers){
				if(header.indexOf(":") > 0){
					String[] param = header.split(":", 2);
					result.put(param[0], param[1]);
				}
			}
		}
		
		return result;
	}
	
	private ChartInfomation createChartInformation(String header){
		if(header.startsWith("Chart:")){
			String style = header.substring(6);
			style = style.trim().toLowerCase();
			if(style.equals(ChartInfomation.STYLE_STACK)){
				return new StackedChartInfomation();
			}else if(style.equals(ChartInfomation.STYLE_PIE)){
				return new PieChartInfomation();
			}else if(style.equals(ChartInfomation.STYLE_STACK_LINE)){
				return new StackedLineChartInfomation();
			}
			
			echo("Not supported chart:" + header.substring(6));
		}else {
			echo("Invalid chart header:" + header);
		}
		
		return null;
	}
	
	private boolean initDirectory(){
		String rootPath = System.getenv("REPORT_ROOT");
		if (rootPath == null) rootPath = System.getProperty("REPORT_ROOT", ".");
		
		workRoot = newFile(rootPath, cmd.getOptionValue(ROOT, "."));
		
		if(!workRoot.isDirectory()){
			echo(String.format("Not found directory '%s'.", workRoot.getAbsolutePath()));
			return false;
		}
		
		if(cmd.hasOption(OUTPUT)){
			outputRoot = newFile(rootPath, cmd.getOptionValue(OUTPUT));
		}else {
			outputRoot = workRoot;
		}
		
		if(outputRoot.getName().indexOf(".") > 0){
			outputFile = outputRoot;
			outputRoot = outputRoot.getParentFile();
		}else if(!outputRoot.isDirectory()){
			outputRoot.mkdirs();
		}
		
		if(outputFile == null) outputFile = new File(outputRoot, "chart" + outExt);
		
		try {
			echo(String.format("input directory \"%s\"", workRoot.getCanonicalPath()));
			echo(String.format("output directory \"%s\"", outputRoot.getCanonicalPath()));
		} catch (IOException e) {
			echo("Error:" + e.getMessage());
			exception(e);
		}
		
		return true;
	}
	
	private File newFile(String root, String file){
		if(file.indexOf(":") > 0 || file.indexOf(File.separator) == 0){
			return new File(file);
		}else {
			return new File(root, file);
		}
	}
	
	private void echo(String s){
		System.out.println(s);
	}

	private void exit(int code){
		System.exit(code);
	}
	
	private void exception(Exception e){
		if(this.debug) {
			e.printStackTrace(System.err);
		}
	}
	
}
