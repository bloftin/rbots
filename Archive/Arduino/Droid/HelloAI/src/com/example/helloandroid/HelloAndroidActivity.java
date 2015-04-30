package com.example.helloandroid;

import android.app.Activity;
import aima.core.environment.tictactoe.TicTacToe;
import aima.core.environment.tictactoe.TicTacToeBoard;
import aima.core.search.adversarial.GameState;
import android.os.Bundle;
import android.widget.TextView;
import android.widget.ScrollView;

public class HelloAndroidActivity extends Activity {
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
             
        setContentView(R.layout.main);
        TextView tv = (TextView) findViewById(R.id.txtView);
        //tv.append("TicTacToe Demo\n\n");
		//minimaxDemo();
		alphaBetaDemo(tv);
		
        
    }
    
	private static void alphaBetaDemo(TextView view) {
		view.append("ALPHA BETA\n\n");
		TicTacToe t4 = new TicTacToe();
		while (!(t4.hasEnded())) {
			view.append("\n" + t4.getPlayerToMove(t4.getState())
					+ "  playing ... \n");

			t4.makeAlphaBetaMove();
			GameState presentState = t4.getState();
			TicTacToeBoard board = t4.getBoard(presentState);
			//board.print();  -- put our own print to view here, better to override function...
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++)
					view.append(board.getValue(i, j) + " ");
				view.append("\n");
			}
		}
		view.append("\nALPHA BETA DEMO done\n");
	}

	private static void minimaxDemo() {
		System.out.println("MINI MAX ");
		System.out.println("");
		TicTacToe t3 = new TicTacToe();
		while (!(t3.hasEnded())) {
			System.out.println("\n" + t3.getPlayerToMove(t3.getState())
					+ " playing");
			System.out.println("");
			t3.makeMiniMaxMove();
			GameState presentState = t3.getState();
			TicTacToeBoard board = t3.getBoard(presentState);
			System.out.println("");
			board.print();

		}
		System.out.println("Mini MAX DEMO done");
	}
}