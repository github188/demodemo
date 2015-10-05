### Set Maven proxy ###
add node in pom.xml
```
```

### Create hudson plugin project ###

$ mvn -cpu hpi:create

### Set plugins name ###
add node in pom.xml

`<`name`>`Robot project schedule plugin`<`/name`>`

### Add navigation link in Project/build ###

1. what's the usage of Recorder?
2. How the extension is discovered by Hudson?
the plugin is discovered and auto-registered by added 'Extension' annotation.
the annotation supports be added to static field, factory method and class.

```

package hudson.plugins.robotschedule;

import hudson.Extension;
import hudson.Launcher;
import hudson.model.AbstractBuild;
import hudson.model.AbstractProject;
import hudson.model.Action;
import hudson.model.BuildListener;
import hudson.model.Project;
import hudson.tasks.BuildStepDescriptor;
import hudson.tasks.BuildStepMonitor;
import hudson.tasks.Publisher;
import hudson.tasks.Recorder;

import org.kohsuke.stapler.DataBoundConstructor;

public class RobotFrameworkResultPublisher extends Recorder {

    private final String testExecutionResultPath;

    @DataBoundConstructor
    public RobotFrameworkResultPublisher(String path) {
        this.testExecutionResultPath = path;
    }

    public String getPath() {
        return testExecutionResultPath;
    }

    @Override
    public Action getProjectAction(Project project) {
        return new RobotFrameworkProject2Action(project, testExecutionResultPath);
    }
    
    @Override
    public boolean perform(AbstractBuild<?, ?> build, Launcher launcher, BuildListener listener) {
        build.addAction(new RobotFrameworkBuildAction(build, testExecutionResultPath));
        return true;
    }

    @Override
    public DescriptorImpl getDescriptor() {
        return (DescriptorImpl) super.getDescriptor();
    }

    @Extension
    public static final class DescriptorImpl extends BuildStepDescriptor<Publisher> {
        public boolean isApplicable(Class<? extends AbstractProject> aClass) {
            return true;
        }

        public String getDisplayName() {
            return "Publish Robot schedule test results";
        }
    }

    
    public BuildStepMonitor getRequiredMonitorService() {
        return BuildStepMonitor.NONE;
    }
}

```

### How to register the plugin by descriptor ###
{@link Descriptor} is an object that has metadata about a {@link Describable}
object, and also serves as a factory (in a way this relationship is similar
to {@link Object}/{@link Class} relationship.

A {@link Descriptor}/{@link Describable}
combination is used throughout in Hudson to implement a
configuration/extensibility mechanism.

```
    protected Descriptor() {
        this.clazz = (Class<T>)getClass().getEnclosingClass();
        if(clazz==null)
            throw new AssertionError(getClass()+" doesn't have an outer class. Use the constructor that takes the Class object explicitly.");

        // detect an type error
        Type bt = Types.getBaseClass(getClass(), Descriptor.class);
        if (bt instanceof ParameterizedType) {
            ParameterizedType pt = (ParameterizedType) bt;
            // this 't' is the closest approximation of T of Descriptor<T>.
            Class t = Types.erasure(pt.getActualTypeArguments()[0]);
            if(!t.isAssignableFrom(clazz))
                throw new AssertionError("Outer class "+clazz+" of "+getClass()+" is not assignable to "+t+". Perhaps wrong outer class?");
        }

        // detect a type error. this Descriptor is supposed to be returned from getDescriptor(), so make sure its type match up.
        // this prevents a bug like http://www.nabble.com/Creating-a-new-parameter-Type-%3A-Masked-Parameter-td24786554.html
        try {
            Method getd = clazz.getMethod("getDescriptor");
            if(!getd.getReturnType().isAssignableFrom(getClass())) {
                throw new AssertionError(getClass()+" must be assignable to "+getd.getReturnType());
            }
        } catch (NoSuchMethodException e) {
            throw new AssertionError(getClass()+" is missing getDescriptor method.");
        }

    }
```

### Add navigation link in Hudson ###


### Doing Describable/Descriptor pattern ###
http://wiki.hudson-ci.org/display/HUDSON/Defining+a+new+extension+point

### Debugging a Plugin ###
Windows:
```
> set MAVEN_OPTS=-Xdebug -Xrunjdwp:transport=dt_socket,server=y,address=8000,suspend=n
> mvn hpi:run
```