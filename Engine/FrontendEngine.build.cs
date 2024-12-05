using System;
using System.IO;
using Sharpmake;

[module: Include("Source/Utils.cs")]
[module: Include("Source/CommonProject.build.cs")]

[Generate]
public class FrontendEngineProject : CommonProject
{
    public FrontendEngineProject() : base(false)
    {
        Name = "SE";  // 프로젝트 이름을 "SE"로 설정

        // .ini, .hlsl 파일을 소스 파일 확장자에 추가
        SourceFilesExtensions.Add(".ini");
        SourceFilesExtensions.Add(".hlsl");
        StripFastBuildSourceFiles = false; // FastBuild에서 소스 파일을 제외하지 않음

        // 타겟을 추가 (여기서는 엔진 타겟을 추가)
        AddTargets(new EngineTarget(
            ELaunchType.Editor | ELaunchType.Client | ELaunchType.Server,
            Platform.win64,
            DevEnv.vs2022,
            Optimization.Debug | Optimization.Release,
            OutputType.Lib,
            Blob.NoBlob,
            BuildSystem.FastBuild
        ));
    }

    public override void ConfigureAll(Configuration conf, EngineTarget target)
    {
        base.ConfigureAll(conf, target);

        //conf.TargetFileFullNameWithExtension = conf.ProjectName + ".exe";
        conf.TargetFileFullNameWithExtension = "SE1.exe";

        conf.FastBuildMakeCommand = "";//"msbuild Intermediate\\ProjectFiles\\EngineConfig.vcxproj /t:Rebuild /p:platform=x64 /p:configuration=\"" + conf.Name + "\"\n";
        conf.FastBuildMakeCommand += "if exist \"Intermediate\\ProjectFiles\\UserConfig.vcxproj\" ( msbuild Intermediate\\ProjectFiles\\UserConfig.vcxproj /t:Rebuild /p:platform=x64 /p:configuration=\"" + conf.Name + "\")\n";
        conf.FastBuildMakeCommand += "devenv Intermediate\\ProjectFiles\\Engine.sln /Build \"" + conf.Name + "\"\n";

        if (ELaunchType.Editor == target.LaunchType)
        {
            conf.FastBuildMakeCommand += "if exist \"Intermediate\\ProjectFiles\\GameProject.vcxproj\" ( devenv Intermediate\\ProjectFiles\\GameProject.vcxproj /Build \"" + conf.Name + "\")\n";
        }
        
        conf.FastReBuildMakeCommand = "devenv Intermediate\\ProjectFiles\\Engine.sln /Rebuild \"" + conf.Name + "\"";
        conf.IsFastBuild = true;
        conf.SolutionFolder = @"Engine";
    }
}

[Generate]
public class FrontendEngineSolution : Solution
{
    public FrontendEngineSolution() : base(typeof(EngineTarget))
    {
        IsFileNameToLower = false;
        Name = "SE";

        AddTargets(new EngineTarget(
            ELaunchType.Editor | ELaunchType.Client | ELaunchType.Server,
            Platform.win64,
            DevEnv.vs2022,
            Optimization.Debug | Optimization.Release,
            OutputType.Lib,
            Blob.NoBlob,
            BuildSystem.FastBuild
        ));
    }

    [Configure()]
    public virtual void ConfigureAll(Configuration conf, EngineTarget target)
    {
        Utils.MakeConfiturationNameDefine(conf, target);

        conf.SolutionPath = Utils.GetSolutionDir();

        string ProjectFilesDir = conf.SolutionPath + @"\Intermediate\ProjectFiles";
        Environment.SetEnvironmentVariable("ProjectFilesDir", ProjectFilesDir);

        conf.AddProject<FrontendEngineProject>(target);
        conf.SetStartupProject<FrontendEngineProject>();
    }
}