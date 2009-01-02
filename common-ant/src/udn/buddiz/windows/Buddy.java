/*
 * Copyright (c) 2006 Ubiquity Software Corporation plc. All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * - Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer. 
 *  
 * - Redistributions in binary form must reproduce the above copyright notice, 
 * this list of conditions and the following disclaimer in the documentation 
 * and/or other materials provided with the distribution. 
 * 
 * - The name of Ubiquity Software Corporation may not be used to endorse or 
 * promote products derived from this software without specific prior written 
 * permission.
 * 
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITH ALL FAULTS AND WITHOUT
 * ANY TECHNICAL SUPPORT OR WARRANTIES OF ANY KIND. YOU ASSUME ALL RISK OF USE
 * OF THIS SOFTWARE. TO THE MAXIMUM EXTENT PERMITTED BY APPLICABLE LAW,
 * UBIQUITY, ITS SUPPLIERS AND RESELLERS DISCLAIM ALL WARRANTIES WITH RESPECT TO
 * THE TO THE SOFTWARE WHETHER EXPRESS, IMPLIED, STATUTORY OR OTHERWISE,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * NONINFRINGEMENT, QUALITY OF PERFORMANCE AND FITNESS FOR A PARTICULAR PURPOSE.
 * TO THE MAXIMUM EXTENT PERMITTED BY APPLICABLE LAW, IN NO EVENT WILL UBIQUITY,
 * OR ITS SUPPLIERS OR RESELLERS BE LIABLE FOR ANY INDIRECT, SPECIAL, INCIDENTAL
 * OR CONSEQUENTIAL DAMAGES ARISING OUT OF OR RELATED TO THE USE OF OR INABILITY
 * TO USE THE SOFTWARE EVEN IF ADVISED OF THE POSSIBILITY THEREOF, AND
 * REGARDLESS OF THE LEGAL OR EQUITABLE THEORY (CONTRACT, TORT OR OTHERWISE)
 * UPON WHICH THE CLAIM IS BASED. TO THE MAXIMUM EXTENT PERMITTED BY APPLICABLE
 * LAW, IN NO EVENT WILL UBIQUITY, OR ITS SUPPLIERS OR RESELLERS BE LIABLE FOR
 * DAMAGES FOR LOSS OF GOODWILL, LOST DATA, WORK STOPPAGE, COMPUTER FAILURE OR
 * MALFUNCTION, OR ANY AND ALL OTHER COMMERCIAL DAMAGES OR LOSSES, EVEN IF
 * ADVISED OF THE POSSIBILITY THEREOF, AND REGARDLESS OF THE LEGAL OR EQUITABLE
 * THEORY (CONTRACT, TORT OR OTHERWISE) UPON WHICH THE CLAIM IS BASED.
 * UBIQUITY'S TOTAL LIABILITY FOR THE SOFTWARE WILL BE LIMITED TO THE LICENSE
 * FEES PAID BY YOU TO UBIQUITY FOR THE SOFTWARE.
 * 
 * Contact: Ubiquity Software Corporation plc, Building 3, Eastern Business
 * Park, Wern Fawr Lane, St Mellons, Cardiff CF3 5EA United Kingdom, Attention:
 * Technical Services and Operation Manager.
 */
package udn.buddiz.windows;

public class Buddy {
	/** Field displayName. */
	private String displayName;

	/** Field sipAddress. */
	private String sipAddress;

	private Mood mood = Mood.Unknown;

	public Buddy(String buddyString) {
		int posStartBracket = buddyString.indexOf("<");
		if(posStartBracket==-1) {
			displayName = null;
			sipAddress = buddyString;
			return;
		}
		int posEndBracket = buddyString.indexOf(">");
		if(posEndBracket==-1) {
			displayName = null;
			sipAddress = buddyString;
			return;
		}
		
		displayName = buddyString.substring(0, posStartBracket);
		displayName = displayName.trim();
		sipAddress = buddyString.substring(posStartBracket + 1, posEndBracket);
		sipAddress = sipAddress.trim();
	}
	
	/** 
	 * Getter for sipAddress. 
	 * @return The current value of sipAddress.
	 */
	public String getSipAddress() {
		return sipAddress;
	}

	/**
	 * The setter for sipAddress.
	 * @param newSipAddress The new value for sipAddress.
	 */
	public void setSipAddress(String newSipAddress) {
		sipAddress = newSipAddress;
	}
	
	/** 
	 * Getter for displayName. 
	 * @return The current value of displayName.
	 */
	public String getDisplayName() {
		return displayName;
	}

	/**
	 * The setter for displayName.
	 * @param newDisplayName The new value for displayName.
	 */
	public void setDisplayName(String newDisplayName) {
		displayName = newDisplayName;
	}
	
	@Override
	public String toString() {
		if(displayName != null) 
			return displayName;
		return sipAddress;
	}
	
	@Override
	public boolean equals(Object obj) {
		if(!(obj instanceof Buddy))
			return false;
		return sipAddress.equals(((Buddy)obj).getSipAddress());
	}

	public Mood getMood() {
		return mood;
	}

	public void setMood(Mood mood) {
		this.mood = mood;
	}
}
