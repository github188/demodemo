package udn.buddiz.windows;

import java.awt.Component;

import javax.swing.DefaultListCellRenderer;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.ListCellRenderer;

public class BuddyListCellRenderer extends DefaultListCellRenderer {

	public Component getListCellRendererComponent(JList list,
            Object value,
            int index,
            boolean isSelected,
            boolean cellHasFocus) {
        Component c = super.getListCellRendererComponent(list, value,
                index, isSelected, cellHasFocus);
        
        Buddy buddy = (Buddy)value;

		((JLabel)c).setIcon(Mood.getMoodImage(buddy.getMood()));
		((JLabel)c).setText(buddy.getDisplayName());
		return c;
	}

}
