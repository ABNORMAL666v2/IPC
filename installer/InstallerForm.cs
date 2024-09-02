using SharpCompress.Archives;
using SharpCompress.Common;

namespace IPC_Installer
{
    public partial class InstallerForm : Form
    {
        public static string? ManifestInstallDir = null;
        public static bool IsUtil = false;
        public static string InstallerName = AppDomain.CurrentDomain.FriendlyName;
        public static string TempInstallDir = Path.Combine(Path.GetTempPath(), InstallerName);
        public static string AppResource = "app.7z";
        public static string AppManifest = Path.Combine(TempInstallDir, "manifest.dat");

        public InstallerForm()
        {
            InitializeComponent();
        }

        private void Installer_Load(object sender, EventArgs e)
        {
            WriteResourceToFile(Resource1.app, AppResource);
        }

        public void WriteResourceToFile(byte[] resourceData, string fileName)
        {
            try
            {
                string tempFilePath = Path.Combine(TempInstallDir, fileName);

                if (!Directory.Exists(TempInstallDir))
                    Directory.CreateDirectory(TempInstallDir);

                File.WriteAllBytes(tempFilePath, resourceData);
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Error writing resource to file: {ex.Message}");
            }
        }

        public void InstallButton_Click(object sender, EventArgs e)
        {
            try
            {
                using var archive = ExtractArchive();

                var settingData = LoadConfig(AppManifest);
                if (!settingData.TryGetValue("MANIFESTV", out var version) || Convert.ToInt64(version) < 2)
                {
                    MessageBox.Show("Invalid Manifest");
                    Environment.Exit(0);
                }

                settingData.TryGetValue("archpack", out var archpack);
                settingData.TryGetValue("Package_Name", out var name);
                settingData.TryGetValue("installdir", out var manifestInstDir);

                if (archpack == "true" && !string.IsNullOrEmpty(manifestInstDir) && !string.IsNullOrEmpty(name))
                {
                    var systemArch = Environment.GetEnvironmentVariable("PROCESSOR_ARCHITECTURE");
                    string archDir = systemArch switch
                    {
                        "x86" => "Win32",
                        "AMD64" => "x64",
                        "ARM" => "ARM",
                        "ARM64" => "ARM64",
                        _ => throw new NotSupportedException("Unsupported architecture")
                    };

                    Install(manifestInstDir, name, archDir);
                    if (Directory.Exists(TempInstallDir))
                        Directory.Delete(TempInstallDir, true);
                    Environment.Exit(1);
                }
                else
                {
                    Install(manifestInstDir, name, "Win32");
                    if (Directory.Exists(TempInstallDir))
                        Directory.Delete(TempInstallDir, true);
                    Environment.Exit(1);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Error during installation: {ex.Message}");
            }
        }

        private static IArchive ExtractArchive()
        {
            if (!Directory.Exists(TempInstallDir))
                Directory.CreateDirectory(TempInstallDir);

            var archive = ArchiveFactory.Open(Path.Combine(TempInstallDir, AppResource));
            foreach (var entry in archive.Entries)
            {
                if (!entry.IsDirectory)
                    entry.WriteToDirectory(TempInstallDir, new ExtractionOptions()
                    {
                        ExtractFullPath = true,
                        Overwrite = true
                    });
            }

            return archive;
        }

        private void Install(string pgmInstDir, string filename, string arch)
        {
            if (string.IsNullOrEmpty(pgmInstDir))
            {
                MessageBox.Show("Install directory not found in manifest.");
                return;
            }

            try
            {
                var sourcePath = Path.Combine(TempInstallDir, arch, filename);
                var destPath = Path.Combine(pgmInstDir, filename);

                if (!Directory.Exists(pgmInstDir))
                {
                    Directory.CreateDirectory(pgmInstDir);
                    MessageBox.Show($"Successfully Installled to {filename} @ {pgmInstDir}");
                    AddToPath(pgmInstDir);
                    File.Copy(sourcePath, destPath, true);
                }
                else
                    File.Copy(sourcePath, destPath, true);
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error: " + ex.Message);
            }

        }

        private void AddToPath(string installedPath)
        {
            string PathScript = $@"
                    @echo off
                    setx path ""%PATH%;{installedPath}""
                    echo Set path successfully.
                    ping 127.0.0.1 -w 4 > NULL
                    exit";

            string PathFileName = Path.Combine(Path.GetTempPath(), Guid.NewGuid().ToString() + ".bat");
            File.WriteAllText(PathFileName, PathScript);
            var process = System.Diagnostics.Process.Start(PathFileName);
            process?.WaitForExit();
            File.Delete(PathFileName);
        }

        public static Dictionary<string, string> LoadConfig(string settingFile)
        {
            var config = new Dictionary<string, string>();

            if (File.Exists(settingFile))
            {
                var settingsData = File.ReadAllLines(settingFile);
                foreach (var settingLine in settingsData)
                {
                    var settingParts = settingLine.Split('=');
                    if (settingParts.Length == 2)
                    {
                        var settingKey = settingParts[0];
                        var settingValue = settingParts[1];
                        if (!config.ContainsKey(settingKey))
                            config.Add(settingKey, settingValue);
                    }
                }
            }
            else
                MessageBox.Show("Invalid manifest.");
            return config;
        }
    }
}
