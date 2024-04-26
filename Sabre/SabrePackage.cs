﻿using Microsoft.VisualStudio.Shell;
using Sabre.Views.ByteView;
using System;
using System.Runtime.InteropServices;
using System.Threading;
using Task = System.Threading.Tasks.Task;

namespace Sabre
{
    /// <summary>
    /// This is the class that implements the package exposed by this assembly.
    /// </summary>
    /// <remarks>
    /// <para>
    /// The minimum requirement for a class to be considered a valid package for Visual Studio
    /// is to implement the IVsPackage interface and register itself with the shell.
    /// This package uses the helper classes defined inside the Managed Package Framework (MPF)
    /// to do it: it derives from the Package class that provides the implementation of the
    /// IVsPackage interface and uses the registration attributes defined in the framework to
    /// register itself and its components with the shell. These attributes tell the pkgdef creation
    /// utility what data to put into .pkgdef file.
    /// </para>
    /// <para>
    /// To get loaded into VS, the package must be referred by &lt;Asset Type="Microsoft.VisualStudio.VsPackage" ...&gt; in .vsixmanifest file.
    /// </para>
    /// </remarks>
    [PackageRegistration(UseManagedResourcesOnly = true, AllowsBackgroundLoading = true)]
    [Guid(SabrePackage.PackageGuidString)]
    [ProvideMenuResource("Menus.ctmenu", 1)]
    [ProvideToolWindow(typeof(Sabre.Views.Loader.LoaderView))]
	[ProvideToolWindow(typeof(Sabre.Views.ByteView.ByteViewView))]
	[ProvideToolWindow(typeof(Sabre.Views.FunctionView.FunctionViewView))]
	[ProvideToolWindow(typeof(Sabre.Views.PseudoC.PCView))]
	[ProvideToolWindow(typeof(Sabre.Views.StringViewView))]
	public sealed class SabrePackage : AsyncPackage
    {
        /// <summary>
        /// SaberPackage GUID string.
        /// </summary>
        public const string PackageGuidString = "0b1f213b-3690-4155-8722-d31177fdb5cb";

        public static SabrePackage s_Instance { get; private set; }

        #region Package Members

        /// <summary>
        /// Initialization of the package; this method is called right after the package is sited, so this is the place
        /// where you can put all the initialization code that rely on services provided by VisualStudio.
        /// </summary>
        /// <param name="cancellationToken">A cancellation token to monitor for initialization cancellation, which can occur when VS is shutting down.</param>
        /// <param name="progress">A provider for progress updates.</param>
        /// <returns>A task representing the async work of package initialization, or an already completed task if there is none. Do not return null from this method.</returns>
        protected override async Task InitializeAsync(CancellationToken cancellationToken, IProgress<ServiceProgressData> progress)
        {
            s_Instance = this;

			// When initialized asynchronously, the current thread may be a background thread at this point.
			// Do any initialization that requires the UI thread after switching to the UI thread.
			await this.JoinableTaskFactory.SwitchToMainThreadAsync(cancellationToken);

            await Views.Loader.LoaderCommand.InitializeAsync(this);
            await Views.ByteView.ByteViewCommand.InitializeAsync(this);
            await Sabre.Views.FunctionView.FunctionViewCommand.InitializeAsync(this);
            await Sabre.Views.PseudoC.PCCommand.InitializeAsync(this);
            await Sabre.Views.StringViewCommand.InitializeAsync(this);
        }
        
        #endregion
    }
}
