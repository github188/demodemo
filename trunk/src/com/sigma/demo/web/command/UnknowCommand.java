package com.sigma.demo.web.command;

import java.io.IOException;

import com.sigma.web.IndexList;

public class UnknowCommand extends IndexList {

	public String doCommand() throws IOException {

		return "/index.jsp";
	}
}
