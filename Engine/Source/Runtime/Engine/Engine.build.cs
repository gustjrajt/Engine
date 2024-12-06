// Copyright (c) Ubisoft. All Rights Reserved.
// Licensed under the Apache 2.0 License. See LICENSE.md in the project root for license information.
using System;
using System.IO;
using Sharpmake;

[Generate]
public class Engine : CommonProject
{
    public Engine() { }

    public override void ConfigureAll(Configuration conf, EngineTarget target)
    {
        base.ConfigureAll(conf, target);

        
        conf.AddPublicDependency<Core>(target);         // Launch모듈에 Core 모듈 추가(include PATH)
        //conf.AddPublicDependency<Engine>(target);
        //if (target.LaunchType == ELaunchType.Editor)
        //{
        //    conf.AddPublicDependency<RHI>(target);
        //    conf.AddPublicDependency<Slate>(target);
        //}
        //conf.AddPublicDependency<EngineSettings>(target);
        //if (target.LaunchType == ELaunchType.Editor)
        //{
        //    conf.AddPublicDependency<EngineEd>(target);
        //}
    }
}