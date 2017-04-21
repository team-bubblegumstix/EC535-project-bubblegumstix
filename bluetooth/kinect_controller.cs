using System;
using System.IO;
using System.Windows;
using System.Threading;
using Microsoft.Kinect;
using Microsoft.Kinect.Toolkit;
using InTheHand.Net;
using InTheHand.Net.Bluetooth;
using InTheHand.Net.Sockets;


namespace Simple_Kinect
{
    public class Program
    {
        private static void kinect_SkeletonFrameReady(object sender, SkeletonFrameReadyEventArgs e)
        {
            Skeleton[] skeletons = new Skeleton[0];

            using (SkeletonFrame skeletonFrame = e.OpenSkeletonFrame())
            {
                if (skeletonFrame != null)
                {
                    skeletons = new Skeleton[skeletonFrame.SkeletonArrayLength];
                    skeletonFrame.CopySkeletonDataTo(skeletons);
                }
            }

            if (skeletons.Length != 0)
            {
                foreach ( Skeleton skel in skeletons)
                {
                    if (skel.TrackingState == SkeletonTrackingState.Tracked)
                    {
                        SkeletonPoint leftWrist = skel.Joints[JointType.WristLeft].Position;
                        SkeletonPoint rightWrist = skel.Joints[JointType.WristRight].Position;
                        SkeletonPoint hipCenter = skel.Joints[JointType.HipCenter].Position;

                        int hipCenter_Z = Convert.ToInt32(hipCenter.Z * 100);
                        int leftWrist_Z = (hipCenter_Z - Convert.ToInt32(leftWrist.Z * 100)) < 0 ? 0 : hipCenter_Z - Convert.ToInt32(leftWrist.Z * 100);
                        int leftWrist_Y = (Convert.ToInt32(leftWrist.Y * 100)) < 0 ? 0 : Convert.ToInt32(leftWrist.Y * 100);
                        int rightWrist_Z = (hipCenter_Z - Convert.ToInt32(rightWrist.Z * 100)) < 0 ? 0 : hipCenter_Z - Convert.ToInt32(rightWrist.Z * 100);
                        int rightWrist_Y = (Convert.ToInt32(rightWrist.Y * 100)) < 0 ? 0 : Convert.ToInt32(rightWrist.Y * 100);
                        

                        Console.WriteLine("{0} | {1} | {2} | {3} | {4}", leftWrist_Z, leftWrist_Y, rightWrist_Z, rightWrist_Y, hipCenter_Z);

                        try
                        {
                            for (int i = 1; i < 3; i++)
                            {
                                Console.WriteLine("Trying to connect...");
                                BluetoothAddress addr = BluetoothAddress.Parse("00:80:37:2e:31:20");
                                Guid serviceClass;
                                serviceClass = BluetoothService.BluetoothBase;
                                var ep = new BluetoothEndPoint(addr, serviceClass, 2);
                                var cli = new BluetoothClient();

                                cli.Connect(ep);
                                Console.WriteLine("just connected");

                                Stream peerStream = cli.GetStream();
                                if (i == 1)
                                {
                                    
                                    // Player 1s left wrist
                                    string msg = String.Format("{0}{1}{2}", Convert.ToChar(1), Convert.ToChar(leftWrist_Y), Convert.ToChar(leftWrist_Z));
                                    Console.WriteLine("Sending msg");
                                    //string msg = String.Format("{0}{1}{2}{3}", Convert.ToChar(1), Convert.ToChar(100), Convert.ToChar(50), Convert.ToChar(50));
                                    Byte[] to_send = System.Text.Encoding.ASCII.GetBytes(msg);
                                    peerStream.Write(to_send, 0, to_send.Length);
                                    peerStream.Close();
                                }
                                else
                                {
                                    // Player 1s right wrist
                                    string msg = String.Format("{0}{1}{2}", Convert.ToChar(2), Convert.ToChar(rightWrist_Y), Convert.ToChar(rightWrist_Z));
                                    //string msg = String.Format("{0}{1}{2}{3}", Convert.ToChar(1), Convert.ToChar(100), Convert.ToChar(50), Convert.ToChar(100));
                                    Byte[] to_send = System.Text.Encoding.ASCII.GetBytes(msg);
                                    peerStream.Write(to_send, 0, to_send.Length);
                                    peerStream.Close();
                                }

                            }
                        }
                        catch (Exception)
                        {
                            Console.WriteLine("ERROR: Could not connect to Bluetooth Server");
                        }
                        //Thread.Sleep(100); // slow down the data rate so i can see it
                    }
                }
            }
        }

        static void Main(string[] args)
        {
            KinectSensor my_sensor = null;

            foreach (var potentialSensor in KinectSensor.KinectSensors)
            {
                if (potentialSensor.Status == KinectStatus.Connected)
                {
                    my_sensor = potentialSensor;
                    Console.WriteLine("Connected to the Kinect Camera!");
                    break;
                }
                else
                {
                    Console.WriteLine("No Kinect detected...check USB");
                    return;
                }
            }

            if (my_sensor != null)
            {
                my_sensor.SkeletonStream.Enable(); // Turn on the skeleton stream to receive skeleton frames

                my_sensor.SkeletonFrameReady += new EventHandler<SkeletonFrameReadyEventArgs>(kinect_SkeletonFrameReady); // Get Ready for Skeleton Ready Events                                                                                            

                try
                {
                    my_sensor.Start();
                    Console.WriteLine("Starting the Kinect");
                }
                catch (IOException)
                {
                    return;
                }

                while (true)
                {
                    //run...waiting for kinect interrupts
                }
            }
        }
    }
}
