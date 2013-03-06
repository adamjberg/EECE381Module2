package ubc.eece.graphicsdemo;

import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;

public class MainActivity extends Activity {
	MyNewView my_new_view;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		//my_new_view = new MyNewView(this);
		setContentView(R.layout.activity_main);
	}

	@Override
	protected void onStop() {
		super.onStop();
	}

}
