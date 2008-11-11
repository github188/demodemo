import java.awt.Point;
import java.util.*;

/**
 * 游戏地图文件为.XSB纯文本文件, 构成如下(可参看map附件)
 @ 代表搬运工的起始地点
 # 代表墙
 $ 代表箱子
 . 代表箱子的目的地
 * 代表箱子的初始地点在目的地上
 + 代表搬运工的初始地点在目的地上
 空格 表示地图上的空白
 */

public class PushBox extends Thread{
        private static int nodeCount = 0;
        private static int stepCount = 0;
        private static boolean notFind = true;

        private Map<Integer,PushBox> works = null;
        private	LinkedList<Node> path = new LinkedList<Node>();
        private Point point = null;
        private int step = 0;

        PushBox(Map<Integer,PushBox> works, Node start){
            this.works = works;
            this.point = start.worker;
            this.addNewStatus(start);
            this.setName("T_" + point.x+"_" + point.y);
            System.out.println("New Searcher:" + this.getName());
            this.start();
        }

        public void addNewStatus(Node node){
            if(node.worker.x != point.x ||
               node.worker.y != point.y){
                return;
            }

            synchronized(path){
                for (Node n : path){
                    if(n.equals(node))return;
                }
                path.add(node);
                addNodeCount();
                path.notifyAll();
            }

            if(path.size() % 200 == 0 ||
               nodeCount % 1000 == 0 ||
               stepCount % 1000 == 0){
                System.out.println(this.getName() + " size :" + path.size() +
									", pendding size :" + (path.size() - step) +
                                   ", nodeCount :" + nodeCount +
                                   ", stepCount :" + stepCount);
            }
        }

        public synchronized void addNodeCount(){
            nodeCount++;
        }

        public synchronized void addStepCount() {
            stepCount++;
        }


        public void run(){
            Node cur = null;
            Node lastStatus = null;
            DONE:
            while(notFind){
                synchronized(path){
                    if(path.size() > step){
                        cur = path.get(step++);
                    }else {
                        try {
                            if(checllAllWorks()){
                                path.wait();
                            }
                            continue;
                        } catch (InterruptedException ex) {
                            continue;
                        }
                   }
                }

                for (Direction dir : Direction.DIRS) {
                    addStepCount();
                    if (cur.canMove(dir)) {
                        lastStatus = new Node(cur);
                        lastStatus.moveTo(dir);
                        if (lastStatus.isComplete()) {
                            stopAllWorks();
                            break DONE;
                        }

                        int hashCode = lastStatus.map.length *
                                       lastStatus.worker.x +
                                       lastStatus.worker.y;
                        PushBox task = null;
                        synchronized (works) {
                            task = works.get(hashCode);
                            if(task == null){
                                works.put(hashCode, new PushBox(works, lastStatus));
                            }
                        }
                        if(task != null){
                            task.addNewStatus(lastStatus);
                        }
                    }
                }
            }
            if(lastStatus != null && lastStatus.isComplete()){
                System.out.println("========Result============");
                System.out.println(this.getName() +
                                   ", nodeCount :" + nodeCount +
                                   ", stepCount :" + stepCount);
              Collection<String> steps = findPath(lastStatus);
              System.out.println("Last steps:" + steps.size());
              int i = 0;
                for (String dir : steps) {
                    System.out.print(dir + " ");
                    i++;
                    if(i % 10 == 0)System.out.println();
                }
                System.out.println("\n========End Result============");
            }else {
                System.out.println("Not found in:" + this.getName());
            }
        }

        //检查是否还有线程在运行，如果所有线程都处于等待状态。收索结果。
        private boolean checllAllWorks(){
            synchronized (works) {
                if (notFind) {
                    for (PushBox w : works.values()) {
                        if (w.equals(this))continue;
                        if (w.path.size() > w.step ||
                            !w.getState().equals(State.WAITING))return true;
                    }
                    stopAllWorks();
                }
            }
            return false;
        }

        private synchronized void stopAllWorks(){
            if (notFind) {
                notFind = false;
                for (PushBox w : works.values()) {
                    if (w.equals(this))continue;
                    //System.out.println("Stop:" + w.getName());
                    synchronized (w.path) {
                        w.path.notifyAll();
                    }
                }
            }
        }

	public Collection<String> findPath(Node node){
		Stack<Direction> st = new Stack<Direction>();
		while(node.preNode != null){
			st.push(Direction.compare(node.worker, node.preNode.worker));
			node = node.preNode;
		}
                Collection<String> steps = new Vector<String>();
                for(;!st.isEmpty();steps.add(st.pop().name));
		return steps;
	}
	/*
	0 空格 表示地图上的空白
	1 @ 代表搬运工的起始地点
	2 # 代表墙
	3 $ 代表箱子
	4 . 代表箱子的目的地
	5 * 代表箱子的初始地点在目的地上
	6 + 代表搬运工的初始地点在目的地上
	*/

       public static void main(String args[]) {
   //            PushBox boxWorker = new PushBox();
            byte[][] map = {{2,2,2,2,2,2,2,2},
                            {2,2,2,0,0,0,2,2},
                            {2,0,3,0,2,0,2,2},
                            {2,0,3,4,3,4,0,2},
                            {2,0,2,2,4,0,0,2},
                            {2,0,0,0,0,2,2,2},
                            {2,2,2,2,2,2,2,2}};
   /*
            byte[][] map = {{2,2,2,2,2,2,2},
                            {2,4,0,4,0,4,2},
                            {2,0,3,3,3,0,2},
                            {2,4,3,0,3,4,2},
                            {2,0,3,3,3,0,2},
                            {2,4,0,4,0,4,2},
                            {2,2,2,2,2,2,2}};
     */
//           byte[][] map = { {2, 2, 2, 2, 2},
//                            {2, 4, 3, 0, 2},
//                            {2, 0, 0, 0, 2},
//                            {2, 2, 2, 2, 2}};

           Node start = new Node(map, 5, 3);
           int hashCode = start.map.length *
                          start.worker.x +
                          start.worker.y;
           Map<Integer, PushBox> works = new HashMap<Integer, PushBox>();
           works.put(hashCode,
                     new PushBox(works, start));
       }
}

class Direction{
	public static final Direction LEFT = new Direction(-1,0,"UP");
	public static final Direction RIGHT = new Direction(1,0,"Down");
	public static final Direction UP = new Direction(0,-1,"Left");
	public static final Direction DOWN = new Direction(0,1,"Right");
	public static final Direction[] DIRS = {LEFT,RIGHT,UP,DOWN};

	public final int x, y;
	public String name;
	public Direction(int x, int y,String name){
		this.x = x;
		this.y = y;
		this.name = name;
	}

	public static Direction compare(Point p1, Point p2){
		int dx = p1.x - p2.x;
		int dy = p1.y - p2.y;
		for(Direction d : DIRS){
			if(d.x == dx && d.y == dy){
				return d;
			}
		}
		return null;
	}
}

	/*
	0 空格 表示地图上的空白
	1 @ 代表搬运工的起始地点
	2 # 代表墙
	3 $ 代表箱子
	4 . 代表箱子的目的地
	5 * 代表箱子的初始地点在目的地上
	6 + 代表搬运工的初始地点在目的地上
	*/
class Node{
	Point worker = new Point();
	byte[][] map = null;  //
	Node preNode = null;
        String log = "";
	public Node(byte[][] map, int workx, int worky){
		this.map = map;
		worker.x = workx;
		worker.y = worky;
	}

	public Node(Node old){
		this.worker.x = old.worker.x;
		this.worker.y = old.worker.y;
		preNode = old;
		map = old.map.clone();
                for(int i = 0; i < map.length; i++){
                    map[i] = map[i].clone();
                }
	}

	public void moveTo(Direction dir){
		int x, y;
                worker.x += dir.x;
                worker.y += dir.y;
                if(haveBox(worker.x,worker.y)){
                        map[worker.x][worker.y] = (byte)((map[worker.x][worker.y] == 5) ? 4 : 0);
                        x = worker.x + dir.x;
                        y = worker.y + dir.y;
                        map[x][y] = (byte)((map[x][y] == 4) ? 5 : 3);
                }
	}

	public boolean canMove(Direction dir){
		int x, y;
		x = worker.x + dir.x;
		y = worker.y + dir.y;
		if(isLegal(x,y) && map[x][y] != 2){ //方向上不是墙。
			if(haveBox(x,y)){
				x = x+dir.x; //移动已存在的箱子。
				y = y+dir.y;
				if(isLegal(x,y) && map[x][y] != 2 && !haveBox(x,y)){
					return true;
				}
			}else {  //空地。
				return true;
			}
		}

		return false;
	}

	public boolean isComplete(){  //判断是否移动结束。
		for(byte[] row: map){
			for(byte bit: row){
				if(bit == 3) return false;
			}
		}
		return true;
	};

	public boolean equals(Object o){
		Node node = (Node)o;
		if(node.worker.x != worker.x || node.worker.y != worker.y) return false;
		for(int i = 0; i< map.length; i++){
			for(int j = 0; j < map[i].length; j++){
				if(node.map[i][j] != map[i][j]) return false;
			}
		}
		return true;
	}

	private boolean isLegal(int x, int y){
		return x < map.length && y < map[0].length && x >= 0 && y >= 0;
	}

	private boolean haveBox(int x, int y){
		return map[x][y] == 3 || map[x][y] == 5;
	}

}
