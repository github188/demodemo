/* MsgFlag.java
 *
 * Sigma Resources & Technologies, Inc.
 * Copyright (C) 2005-2007, sigma-rt.com
 *
 * PROPRIETARY RIGHTS of Sigma Resources & Technologies are involved in
 * the subject matter of this material. All manufacturing, reproduction,
 * use, and sales rights pertaining to this subject matter are governed
 * by the license agreement. The recipient of this software implicitly
 * accepts the terms of the license.
 *
 * Created on Oct 22, 2007, by Alexandre Tsu.
 * 
 * Project:	DemoLauncher
 * Author :	James Che
 * Auditor: 
 * 
 * # any description here # 
 *
 * For more information, visit:
 * http://www.sigma-rt.com
 * 
 * Change History
 * ---------------------------------------------------
 * REVISION          DATE/TIME        AUTHOR
 * ---------------------------------------------------
 * $Log$
 * ---------------------------------------------------
 *
 */
package com.sigma.demo.data;

/**
 * @author James Che
 * 
 */
public interface MsgFlag
{
    static final int MSG_BASE = 100;

    /**
     * <pre><code>
     * [Description]: the environment of the demo launcher
     * [Request]:
     * [Return]: Environment
     * </code></pre>
     */
    public static final int DEMO_Environment = MSG_BASE + 1;

    /**
     * <pre><code>
     * [Description]: the name list for all demo
     * [Request]:
     * [Return]: List<String(demo full name),String(server address),int(server port)>
     * </code></pre>
     */
    public static final int DEMO_LIST = MSG_BASE + 2;

    /**
     * <pre><code>
     * [Description]: the statuses of the specified demos
     * [Request]: String[](demo name) ,request all demo status when the paramter is null
     * [Return]: DemoStatus[]
     *          demo status : DemoStatus.AVAILABLE,DemoStatus.UNAVALIABLE,DemoStatus.OCCUPIED
     * </code></pre>
     */
    public static final int DEMO_STATUSES = MSG_BASE + 3;

    /**
     * <pre><code>
     * [Description]: DemoData data
     * [Request]: String(demo name) 
     * [Return]: DemoData
     * </code></pre>
     */
    public static final int DEMO_DATA = MSG_BASE + 4;

    /**
     * <pre><code>
     * [Description]: Run step of the specified demo
     * [Request]: String(full demo name),String(step name) 
     * [Return]: StepResult
     * </code></pre>
     */
    public static final int STEP_RUN = MSG_BASE + 5;

    /**
     * <pre><code>
     * [Description]: Stop step of the specified demo
     * [Request]: String(full demo name),String(step name) 
     * [Return]: String(server address),int(server port),StepResult
     * </code></pre>
     */
    public static final int STEP_STOP = MSG_BASE + 6;

    /**
     * <pre><code>
     * [Description]: The return message when excuting step of the specified demo
     * [Request]: 
     * [Return]:  // TODO
     * </code></pre>
     */
    public static final int STEP_EXECUTING = MSG_BASE + 7;

    /**
     * <pre><code>
     * [Description]: The return graph data when excuting step of the specified demo
     * [Request]: 
     * [Return]:  SeriesData
     * </code></pre>
     */
    public static final int STEP_GRAPH_DATA = MSG_BASE + 8;

    /**
     * <pre><code>
     * [Description]: Ready to run demo
     * [Request]: String(demo full name),boolean(isCoerce execute)
     * [Return]:  DemoResult
     * </code></pre>
     */
    public static final int DEMO_READY = MSG_BASE + 9;

    /**
     * <pre><code>
     * [Description]: Release demo
     * [Request]: String(demo full name)
     * [Return]:  DemoResult
     * </code></pre>
     */
    public static final int DEMO_RELEASE = MSG_BASE + 10;

    /**
     * <pre><code>
     * [Description]: Login to the remote server
     * [Request]:
     * [Return]: Result
     * </code></pre>
     */
    public static final int DEMO_LOGIN = MSG_BASE + 11;
    
    /**
     * <pre><code>
     * [Description]: The step invoke to prepare
     * [Request]: String(step name),String(demo full name)
     * [Return]: StepResult
     * </code></pre>
     */
    public static final int STEP_PRE = MSG_BASE + 12;
    
    /**
     * <pre><code>
     * [Description]: The stop demo by scheduler
     * [Request]: 
     * [Return]: DemoResult
     * </code></pre>
     */
    public static final int DEMO_STOP_SCHEDULER = MSG_BASE + 13;

    /**
     * <pre><code>
     * [Description]: the heartbeat test
     * [Request]: 
     * [Return]: 
     * </code></pre>
     */
    public static final int HEARTBEAT = MSG_BASE + 50;

    /**
     * <pre><code>
     * [Description]: the console command
     * [Request]: String(demo full name),String(step name),String(shell name),boolean(synchronize),String(commond)
     * [Return]: String(demo full name),String(step name),String(shell name),String(execute result)
     * </code></pre>
     */
    public static final int SHELL_CMD = MSG_BASE + 100;

    /**
     * <pre><code>
     * [Description]: the system quit
     * [Request]: 
     * [Return]: 
     * </code></pre>
     */
    public static final int SYSTEM_QUIT = MSG_BASE + 400;

    /**
     * <pre><code>
     * [Description]: System message
     * [Request]: 
     * [Return]:  String(message context)
     * </code></pre>
     */
    public static final int SYSTEM_MSG = MSG_BASE + 401;

    /**
     * <pre><code>
     * [Description]: the custom message
     * [Request]: 
     * [Return]: 
     * </code></pre>
     */
    public static final int CUSTOM_MSG = MSG_BASE + 500;

}
