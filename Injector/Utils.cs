using System;
using System.IO;

namespace KarambitInnit
{
    class Utils
    {
        public static string setupFilesystem()
        {
            bool hadToCreate = false;

            if (!Directory.Exists("tmp"))
            {
                hadToCreate = true;

                try
                {
                    Directory.CreateDirectory("tmp");
                }
                catch (Exception)
                {
                    return "failed";
                }
            }
            if (!Directory.Exists("DLLs"))
            {
                hadToCreate = true;

                try
                {
                    Directory.CreateDirectory("DLLs");
                }
                catch (Exception)
                {
                    return "failed";
                }
            }
            if (!File.Exists("config.json"))
            {
                hadToCreate = true;

                try
                {
                    File.Create("config.json");
                }
                catch (Exception)
                {
                    return "failed";
                }
            }
            if (!File.Exists("hwid.dat"))
            {
                hadToCreate = true;

                try
                {
                    File.Create("hwid.dat");
                }
                catch (Exception)
                {
                    return "failed";
                }
            }

            if (hadToCreate)
            {
                return "created";
            }
            else
            {
                return "skipped";
            }
        }

        public static string time()
        {
            var PST = TimeZoneInfo.FindSystemTimeZoneById("Pacific Standard Time");
            return " [" + TimeZoneInfo.ConvertTime(DateTime.Now, PST).ToString("HH:mm:ss") + "] ";
        }
    }
}
