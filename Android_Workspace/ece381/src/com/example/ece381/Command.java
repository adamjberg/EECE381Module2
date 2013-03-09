package com.example.ece381;

import java.util.ArrayList;

public class Command {

	private int cmd_index;
	private int num_of_parameters;
	private ArrayList<String> parameters;
	
	public Command(int cmd_index) {
		this.cmd_index = cmd_index;
		this.num_of_parameters = 0;
		this.parameters = new ArrayList<String>();
	}
	
	public void addParameter(String para) {
		this.parameters.add(para);
		this.num_of_parameters++;
	}
	
	public String getParameters() {
		int i;
		String result = "";
		for(i = 0; i < this.num_of_parameters; i ++) {
			result += this.parameters.get(i);
			result += " ";
		}
		return result;
	}
}
