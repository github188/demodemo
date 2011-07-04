/*
 * aTunes 1.12.0
 * Copyright (C) 2006-2009 Alex Aranda, Sylvain Gaudard, Thomas Beckers and contributors
 *
 * See http://www.atunes.org/wiki/index.php?title=Contributing for information about contributors
 *
 * http://www.atunes.org
 * http://sourceforge.net/projects/atunes
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

package org.notebook.gui.widget;

import java.awt.Color;
import java.awt.Font;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.swing.BorderFactory;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.SwingUtilities;
import javax.swing.UIManager;
import javax.swing.UIManager.LookAndFeelInfo;
import javax.swing.plaf.ColorUIResource;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.jvnet.lafwidget.LafWidget;
import org.jvnet.lafwidget.utils.LafConstants;
import org.jvnet.substance.SubstanceLookAndFeel;
import org.jvnet.substance.api.SubstanceColorScheme;
import org.jvnet.substance.api.SubstanceConstants;
import org.jvnet.substance.api.SubstanceSkin;

/**
 * The Class LookAndFeelSelector.
 */
public class LookAndFeelSelector {
	private Log log = LogFactory.getLog("ui");

	/** The skins. */
	private Map<String, String> skins = setListOfSkins();

	/** The Constant DEFAULT_SKIN. */
	public final String DEFAULT_SKIN = "aTunes Blue";

	/**
	 * Gets the list of skins.
	 * 
	 * @return the list of skins
	 */
	public List<String> getListOfSkins() {
		List<String> result = new ArrayList<String>(skins.keySet());
		Collections.sort(result, new Comparator<String>() {
			@Override
			public int compare(String o1, String o2) {
				return o1.toLowerCase().compareTo(o2.toLowerCase());
			}
		});
		return result;
	}

	/**
	 * Sets the list of skins.
	 * 
	 * @return the map< string, string>
	 */
	private Map<String, String> setListOfSkins() {
		Map<String, String> result = new HashMap<String, String>();

		/*
		 * toned down skins
		 */
		result.put("BusinessBlackSteel",
				   	"org.jvnet.substance.skin.SubstanceBusinessBlackSteelLookAndFeel");
		result.put("Creme",
  				   	"org.jvnet.substance.skin.SubstanceCremeLookAndFeel");
		result.put("Business",
					"org.jvnet.substance.skin.SubstanceBusinessLookAndFeel");
		result.put("BusinessBlueSteel",
					"org.jvnet.substance.skin.SubstanceBusinessBlueSteelLookAndFeel");
		result.put("CremeCoffee",
					"org.jvnet.substance.skin.SubstanceCremeCoffeeLookAndFeel");
		result.put("Sahara",
					"org.jvnet.substance.skin.SubstanceSaharaLookAndFeel");
		result.put("Moderate",
					"org.jvnet.substance.skin.SubstanceModerateLookAndFeel");
		result.put("OfficeSilver2007",
					"org.jvnet.substance.skin.SubstanceOfficeSilver2007LookAndFeel");
		result.put("Nebula",
					"org.jvnet.substance.skin.SubstanceNebulaLookAndFeel");
		result.put("NebulaBrickWall",
					"org.jvnet.substance.skin.SubstanceNebulaBrickWallLookAndFeel");
		result.put("Autumn",
					"org.jvnet.substance.skin.SubstanceAutumnLookAndFeel");
		result.put("MistSilver",
					"org.jvnet.substance.skin.SubstanceMistSilverLookAndFeel");
		result.put("MistAqua",
					"org.jvnet.substance.skin.SubstanceMistAquaLookAndFeel");

		/*
		 * dark skins
		 */
		result.put("RavenGraphite",
					"org.jvnet.substance.skin.SubstanceRavenGraphiteLookAndFeel");
		result.put("RavenGraphiteGlass",
					"org.jvnet.substance.skin.SubstanceRavenGraphiteGlassLookAndFeel");
		result.put("Raven",
					"org.jvnet.substance.skin.SubstanceRavenLookAndFeel");
		result.put("Magma",
					"org.jvnet.substance.skin.SubstanceMagmaLookAndFeel");
		result.put("ChallengerDeep",
					"org.jvnet.substance.skin.SubstanceChallengerDeepLookAndFeel");
		result.put("EmeraldDusk",
					"org.jvnet.substance.skin.SubstanceEmeraldDuskLookAndFeel");

		/*
		 * satured skins
		 */
		result.put("OfficeBlue2007",
					"org.jvnet.substance.skin.SubstanceOfficeBlue2007LookAndFeel");

		/*
		 * custom skins
		 */
		result.put("aTunes Blue",
					"com.wateray.ipassbook.ui.substance.SubstanceATunesBlueLookAndFeel");
		result.put("aTunes Dark",
					"com.wateray.ipassbook.ui.substance.SubstanceATunesDarkLookAndFeel");
		result.put("aTunes Gray",
					"com.wateray.ipassbook.ui.substance.SubstanceATunesGrayLookAndFeel");

		for (LookAndFeelInfo lf : UIManager.getInstalledLookAndFeels()) {
			result.put(lf.getName(), lf.getClassName());
		}

		return result;
	}

	/**
	 * Sets the look and feel.
	 * 
	 * @param theme
	 *            the new look and feel
	 */
	public void setLookAndFeel(String theme) {
		try {
			log.info("Updating skin '" + theme + "'");
			if (skins.containsKey(theme)) {
				UIManager.setLookAndFeel(skins.get(theme));
			} else {
				log.error("Not found skin '" + theme + "'");
				return;
			}
			
			/** fix font bug start */
			if (SwingUtilities.isEventDispatchThread()) {
				fixFontBug();
			} else {
				SwingUtilities.invokeAndWait(new Runnable() {
					@Override
					public void run() {
						fixFontBug();
					}
				});
			}
		} catch (Exception e) {
			log.error(e.toString(), e);
		}
		
		/**
		 * 如果配色方案不是系统内置的，需要把当前的配色保存，在有些特殊的地方使用。
		 * 例如：设置tiptool的背景色。
		 */
		if (!isDefaultLookAndFeel(theme)) {
			// Get border color
			try {
				Color c = SubstanceLookAndFeel.getCurrentSkin().getMainActiveColorScheme().getMidColor();			
				UIManager.put("ToolTip.border", BorderFactory.createLineBorder(c));
				UIManager.put("ToolTip.background", new ColorUIResource(Color.WHITE));
				UIManager.put("ToolTip.foreground", new ColorUIResource(Color.BLACK));
				
				SubstanceSkin skin = SubstanceLookAndFeel.getCurrentSkin();
				SubstanceColorScheme scheme = skin.getMainActiveColorScheme();
				
				GuiUtils.putLookAndFeelColor("borderColor", scheme.getMidColor());
				GuiUtils.putLookAndFeelColor("lightColor", scheme.getLightColor());
				GuiUtils.putLookAndFeelColor("lightBackgroundFillColor",
											 scheme.getLightBackgroundFillColor());
				GuiUtils.putLookAndFeelColor("darkColor", scheme.getDarkColor());
				GuiUtils.putLookAndFeelColor("backgroundFillColor",
											 scheme.getBackgroundFillColor());
				GuiUtils.putLookAndFeelColor("lineColor", scheme.getLineColor());
				GuiUtils.putLookAndFeelColor("selectionForegroundColor",
											 scheme.getSelectionForegroundColor());
				GuiUtils.putLookAndFeelColor("selectionBackgroundColor",
											 scheme.getSelectionBackgroundColor());
				GuiUtils.putLookAndFeelColor("foregroundColor",
											 scheme.getForegroundColor());
				GuiUtils.putLookAndFeelColor("focusRingColor",
											 scheme.getFocusRingColor());

			} catch (Exception e) {
				log.info("This is not a SubstanceLookAndFeel skin.");
			}

			UIManager.put(LafWidget.ANIMATION_KIND, LafConstants.AnimationKind.NONE);
			UIManager.put(SubstanceLookAndFeel.TABBED_PANE_CONTENT_BORDER_KIND,
						SubstanceConstants.TabContentPaneBorderKind.SINGLE_FULL);
			
			JFrame.setDefaultLookAndFeelDecorated(true);
			JDialog.setDefaultLookAndFeelDecorated(true);
		}
	}

	/**
	 * */
	
	public static boolean isDefaultLookAndFeel(String theme){
		boolean defaultLF = false;
		for (LookAndFeelInfo lf : UIManager.getInstalledLookAndFeels()) {
			// not default lookAndFeel
			if (theme.equals(lf.getName())) {
				defaultLF = true;
			}
		}
		return defaultLF;
	}
	
	private static void fixFontBug() {
		int sizeOffset = 0;
		Enumeration keys = UIManager.getLookAndFeelDefaults().keys();
		while (keys.hasMoreElements()) {
			Object key = keys.nextElement();
			Object value = UIManager.get(key);
			if (value instanceof Font) {
				Font oldFont = (Font) value;
				// logger.info(oldFont.getName());
				Font newFont = new Font("Dialog", oldFont.getStyle(), oldFont
						.getSize()
						+ sizeOffset);
				UIManager.put(key, newFont);
			}
		}
	}

}
