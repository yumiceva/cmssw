package confdb.gui;

import javax.swing.*;
import javax.swing.event.*;
import javax.swing.tree.*;
import java.awt.*;
import java.awt.event.*;

import confdb.data.Directory;
import confdb.data.Configuration;
import confdb.data.ConfigInfo;
    
import confdb.db.ConfDB;
import confdb.db.DatabaseException;


/**
 * DirectoryTreeMouseListener
 * --------------------------
 * @author Philipp Schieferdecker
 *
 * Show a popup menu if a directory is choosen per rigth-click.
 */
public class DirectoryTreeMouseListener extends    MouseAdapter 
                                        implements ActionListener,TreeModelListener
{
    //
    // member data
    //
    
    /** the directory tree being serviced */
    private JTree directoryTree = null;

    /** the directory tree-model */
    private DirectoryTreeModel directoryTreeModel = null;
    
    /** the root directory */
    private Directory rootDir = null;
    
    /** the database which is being described */
    private ConfDB database = null;
    
    /** action commands */
    private static final String ADD_DIRECTORY = new String("Add Directory");
    private static final String RMV_DIRECTORY = new String("Remove Directory");
    
    
    //
    // construction
    //
    
    /** standard constructor */
    public DirectoryTreeMouseListener(JTree directoryTree,ConfDB database)
    {
	this.directoryTree      = directoryTree;
	this.directoryTreeModel = (DirectoryTreeModel)directoryTree.getModel();
	this.rootDir            = (Directory)directoryTreeModel.getRoot();
	this.database           = database;

	directoryTreeModel.addTreeModelListener(this);
    }
    
    
    //
    // member functions
    //
    
    /** MouseAdapter: mousePressed() */
    public void mousePressed(MouseEvent e) { maybeShowPopup(e); }
    
    /** MouseAdapter: mouseReleased() */
    public void mouseReleased(MouseEvent e) { maybeShowPopup(e); }
    
    /** show the popup meny */
    private void maybeShowPopup(MouseEvent e)
    {
	if (!e.isPopupTrigger()) return;
	JTree    tree     = (JTree)e.getComponent();
	TreePath treePath = tree.getPathForLocation(e.getX(),e.getY());
	if (treePath==null) return;
	tree.setSelectionPath(treePath);
	Object o = treePath.getLastPathComponent();
	if (o instanceof Directory) {
	    Directory  selectedDir = (Directory)o;
	    JPopupMenu popup       = new JPopupMenu();
	    
	    JMenuItem  menuItem = new JMenuItem(ADD_DIRECTORY);
	    menuItem.addActionListener(this);
	    menuItem.setActionCommand(ADD_DIRECTORY);
	    popup.add(menuItem);
	    
	    if (selectedDir!=rootDir&&
		selectedDir.childDirCount()==0&&
		selectedDir.configInfoCount()==0){
		menuItem = new JMenuItem(RMV_DIRECTORY);
		menuItem.addActionListener(this);
		menuItem.setActionCommand(RMV_DIRECTORY);
		popup.add(menuItem);
	    }
	    
	    popup.show(e.getComponent(),e.getX(),e.getY());
	}
    }
    
    /** ActionListener: actionPerformed() */
    public void actionPerformed(ActionEvent e)
    {
	System.out.println("actionPerformed()");
	
	String    actionCmd = e.getActionCommand();
	TreePath  treePath  = directoryTree.getSelectionPath();
	Directory selDir    = (Directory)treePath.getLastPathComponent();

	
	if (actionCmd.equals(ADD_DIRECTORY)) {
	    System.out.println("ADD_DIRECTORY");
	    Directory newDir    = new Directory(-1,"<ENTER DIR NAME>","",selDir);
	    
	    selDir.addChildDir(newDir);
	    directoryTreeModel.nodeInserted(selDir,selDir.childDirCount()-1);
	    
	    directoryTree.expandPath(treePath);
	    TreePath newTreePath = treePath.pathByAddingChild(newDir);
	    
	    System.out.println("newTreePath = " + newTreePath);
	    
	    directoryTree.expandPath(newTreePath);
	    directoryTree.scrollPathToVisible(newTreePath);
	    directoryTree.setSelectionPath(newTreePath);
	    directoryTree.startEditingAtPath(newTreePath);
	}
	else if (actionCmd.equals(RMV_DIRECTORY)) {
	    Directory parentDir = selDir.parentDir();
	    int       iChildDir = parentDir.indexOfChildDir(selDir);
	    parentDir.removeChildDir(selDir);
	    directoryTreeModel.nodeRemoved(parentDir,iChildDir,selDir);
	}
    }

    /** TreeModelListener: treeNodesChanged() */
    public void treeNodesChanged(TreeModelEvent e)
    {
	System.out.println("treeNodesChanged()");
	TreePath  treePath  = e.getTreePath();
	System.out.println("treePath = " + treePath);
	if (treePath==null) return;
	int       index     = e.getChildIndices()[0];
	Directory parentDir = (Directory)treePath.getLastPathComponent();
	Directory childDir  = parentDir.childDir(index);
	
	try {
	    System.out.println("parentDir = " + parentDir.name());
	    System.out.println("name = " + childDir.name());
	    database.insertDirectory(childDir);
	    System.out.println("Directory '" + childDir.name() + "' INSERTed!");
	    System.out.println("dbId: " + childDir.dbId());
	}
	catch (DatabaseException ex) {
	    parentDir.removeChildDir(childDir);
	    directoryTreeModel.nodeRemoved(parentDir,
					   parentDir.childDirCount(),
					   childDir);
	    System.err.println(ex.getMessage());
	    ex.printStackTrace();
	}
	
	// want to fire a tree selection event here
	TreeSelectionModel selModel=directoryTree.getSelectionModel();
	TreePath newDirPath = selModel.getSelectionPath();
	selModel.clearSelection();
	selModel.setSelectionPath(newDirPath);
    }
    
    /** TreeModelListener: treeNodesRemoved() */
    public void treeNodesRemoved(TreeModelEvent e)
    {
	Directory dirToBeRemoved = (Directory)(e.getChildren()[0]);
	try {
	    database.removeDirectory(dirToBeRemoved);
	}
	catch (DatabaseException ex) {
	    System.err.println(ex.getMessage());
	}
    }
    
    /** TreeModelListener: treeNodesInserted() */
    public void treeNodesInserted(TreeModelEvent e) {}
    
    /** TreeModelListener: treeStructureChanged() */
    public void treeStructureChanged(TreeModelEvent e) {}
    
}

