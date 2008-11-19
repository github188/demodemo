package org.demo.five.ai;

import org.demo.five.model.DataModel;

public class BaseTestGameTree {
	protected static final byte RC = 1, HC = 2;
	protected GameTree game = null;
	protected DataModel data = null;
	protected int[][] testData1 = {{7,7,RC} };
	
	protected void initChessStatus(int size, int[][] testData){
		data = new DataModel(size);
		game = gameInstance(data);
		
		for(int[] d : testData){
			data.set(d[0], d[1], d[2]);
		}
		
	}	
	
	protected void initChessStatus(int size, String st){
		data = new DataModel(size);
		game = gameInstance(data);
		
		int length = data.data.length;
		for(int i = 0; i < length * length; i++){
			if(st.charAt(i) == '*'){
				data.set(i % length, i / length, 1);
			}else if(st.charAt(i) == 'x'){
				data.set(i % length, i / length, 2);
			}
		}
	}	
	
	protected GameTree gameInstance(DataModel data){
		return new GameTree(data, RC, HC);
	}
	
	protected String R(int x, int y){
		return new String(game.checkResult(x, y, RC)[0]);
	}
	protected String H(int x, int y){
		return new String(game.checkResult(x, y, HC)[0]);
	}	
	
	protected int RE(int x, int y){
		GameTree.Position p = new GameTree.Position(x, y, 0);
		return game.evaluation(p, true);
	}
	protected int HE(int x, int y){
		GameTree.Position p = new GameTree.Position(x, y, 0);
		return game.evaluation(p, false);
	}

	protected GameTree.Position S(int depth, boolean robot){
		//GameTree.Position p = new GameTree.Position(x, y, 0);
		return game.search(depth, robot);
	}
	
}
