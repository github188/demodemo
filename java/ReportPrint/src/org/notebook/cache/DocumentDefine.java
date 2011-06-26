package org.notebook.cache;

import java.awt.Image;
import java.awt.Toolkit;
import java.awt.image.BufferedImage;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.Serializable;
import java.io.Writer;
import java.net.URL;
import java.util.ArrayList;
import java.util.List;

public class DocumentDefine implements Serializable, Cloneable{
	public String name = "";
	public String backGround = "";
	public Image bgImage = null;
	
	public List<InputDataField> fields = new ArrayList<InputDataField>();
	
	public void save(Writer w) throws IOException{
		w.write("name=" + name + "\n");
		w.write("backGround=" + backGround + "\n");
		
		for(InputDataField f: fields){
			w.write("field=" + f.getConfigData() + "\n");
		}
	}
	
	public void load(BufferedReader r) throws IOException{
		for(String l = r.readLine(); l != null; l = r.readLine()){
			l = l.trim();
			if(l.length() == 0)continue;
			if(l.startsWith("#"))continue;
			if(l.indexOf("=") == -1)continue;
			if(l.startsWith("name")){
				name = l.split("=", 2)[1].trim();
			}
			
			if(l.startsWith("backGround")){
				backGround = l.split("=", 2)[1].trim();
			}
			
			if(l.startsWith("field")){
				fields.add(new InputDataField(l.split("=", 2)[1]));
			}
		}
	}
	
	public void setImageURL(URL img){
		if(img != null){
			this.bgImage = Toolkit.getDefaultToolkit().getImage(img);
		}else {
			this.bgImage = new BufferedImage(200, 200, BufferedImage.TYPE_INT_ARGB);
			this.bgImage.getGraphics().drawString("Not found:" + this.backGround, 10, 20);
		}
	}	
}
