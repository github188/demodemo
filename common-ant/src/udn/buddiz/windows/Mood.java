package udn.buddiz.windows;

import java.awt.image.BufferedImage;

import javax.swing.ImageIcon;

public enum Mood {
    Unknown,
    Offline,
    Happy,
    Sad;
        
    public static ImageIcon getMoodImage(Mood mood) {
        if(mood == null) {
            return null;
        }
        String fileName = mood.toString() + ".gif";
        ImageIcon icon = new ImageIcon(Mood.class.getResource("/icons/" + fileName));
        return icon;
    }
}
