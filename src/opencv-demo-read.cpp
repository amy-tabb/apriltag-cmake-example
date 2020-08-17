/* Copyright (C) 2013-2016, The Regents of The University of Michigan.
All rights reserved.
This software was developed in the APRIL Robotics Lab under the
direction of Edwin Olson, ebolson@umich.edu. This software may be
available under alternative licensing terms; contact the address above.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of the Regents of The University of Michigan.
 */

// Amy Tabb
// This is an altered version of opencv_demo.cc from the apriltag library.
// opencv_demo.cc detects apriltags in a video stream. This version detects
// apriltags in imagesfiles (read in) and then outputs those detects in an
// output file.

// also, the input format is the same, but improved. (my opinion)




#include "DirectoryFunctions.hpp"
#include "opencv-demo-read.hpp"


int main(int argc, char *argv[])
{
    int print_help = 0;
    int debug = 0;
    int quiet = 0;
    int video = 0;

    string family_name = "tag36h11";
    int number_threads = 1;
    float decimate_factor = 2.0;
    float blur = 0.0;
    int refine_edges = 0;
    string input_dir = "";
    string output_dir = "";


    while (1)
    {
        static struct option long_options[] =
        {
                {"help",   no_argument,       &print_help, 1},
                {"debug", no_argument,       &debug, 1},
                {"quiet", no_argument,       &quiet, 1},
                {"video", no_argument,       &video, 1},
                {"refine-edges", no_argument,       &refine_edges, 1},
                /* These options don’t set a flag.
                     We distinguish them by their indices. */
                {"input",   required_argument, 0, 'a'},
                {"output",  required_argument, 0, 'b'},
                {"family",   required_argument, 0, 'c'},
                {"threads",  required_argument, 0, 'd'},
                {"decimate", required_argument, 0, 'e'},
                {"blur",  required_argument, 0, 'f'},

        };



        if (print_help == 1){

            cout << "Printing help for opencv-demo-read, August 2020."<< endl;

            cout << std::left << setw(30) << "--help" << "No arguments.  Help." << endl;
            cout << std::left << setw(30) << "--debug" << "No arguments. Enable debugging output (slow)" << endl;
            cout << std::left << setw(30) << "--quiet" << "No arguments. Reduce output" << endl;
            cout << std::left << setw(30) << "--video" << "No arguments. Use video for input." << endl;
            cout << std::left << setw(30) << "--input" << "Input directory (when reading from file)." << endl;
            cout << std::left << setw(30) << "--output" << "Output directory (to write results)." << endl;
            cout << std::left << setw(30) << "--refine-edges" << "No arguments. Spend more time trying to align edges of tags." << endl;
            cout << std::left << setw(30) << "--family" << "Tag family to use, default is tag36h11." << endl;
            cout << std::left << setw(30) << "--threads" << "Number of threads." << endl;
            cout << std::left << setw(30) << "--decimate" << "Decimate input image by this factor, default is 2.0." << endl;
            cout << std::left << setw(30) << "--blur" << "Apply low-pass blur to input, default is 0.0.." << endl;

            exit(1);
        }
        /* getopt_long stores the option index here. */
        int option_index = 0;
        int opt_argument;

        opt_argument = getopt_long (argc, argv, "abcdef",
                long_options, &option_index);

        /* Detect the end of the options. */
        if (opt_argument == -1)
            break;

        switch (opt_argument)
        {
        case 0:
            if (long_options[option_index].flag != 0)
                break;
            printf ("option %s", long_options[option_index].name);
            if (optarg)
                printf (" with arg %s", optarg);
            printf ("\n");
            break;
        case 'a':
            input_dir = optarg;
            break;
        case 'b':
            output_dir = optarg;
            break;
        case 'c':
            family_name = optarg;
            break;
        case 'd':
            number_threads = FromString<int>(optarg);
            break;
        case 'e':
            decimate_factor = FromString<float>(optarg);
            break;
        case 'f':
            blur = FromString<float>(optarg);
            break;
        default:{
            cout << "Argument not found " << optarg << endl;
            exit(1);
        }

        }

    }


    if (!video){
        ///////// CHECK THAT DIRECTORIES EXIST /////////////////
        cout << "No video option selected. " << endl;

        if (IsDirectory(input_dir) == false){
            cout << "cannot access " << input_dir << endl;
            exit(1);
        }

        if (IsDirectory(output_dir) == false){
            cout << "cannot access " << output_dir << endl;
            exit(1);
        }

        EnsureDirHasTrailingBackslash(input_dir);
        EnsureDirHasTrailingBackslash(output_dir);

    }


    // Initialize camera
    VideoCapture cap(0);

    if (video){
        if (!cap.isOpened()) {
            cerr << "Couldn't open video capture device" << endl;
            return -1;
        }
    }


    // Initialize tag detector with options
    apriltag_family_t *tf = NULL;

    vector<string> tag_string{"tag36h11", "tag25h9", "tag16h5", "tagCircle21h7", "tagCircle49h12", "tagStandard41h12",
        "tagStandard52h13", "tagCustom48h12" };
    vector<std::function<apriltag_family_t *()> > tag_create_functions{tag36h11_create, tag25h9_create, tag16h5_create,
        tagCircle21h7_create, tagCircle49h12_create, tagStandard41h12_create, tagStandard52h13_create, tagCustom48h12_create};

    vector<std::function<void(apriltag_family_t *)> > tag_destroy_functions{tag36h11_destroy, tag25h9_destroy, tag16h5_destroy,
        tagCircle21h7_destroy, tagCircle49h12_destroy, tagStandard41h12_destroy, tagStandard52h13_destroy, tagCustom48h12_destroy};

    int tag_index = -1;
    int number_types = int(tag_string.size());

    for (int i = 0; i < number_types; i++){
        if (family_name.compare(tag_string[i]) == 0){
            tag_index = i;
            tf = tag_create_functions[tag_index]();
            i = number_types; // exit the loop
        }
    }

    cout << "Tag family " << family_name << endl;

    if (tag_index == -1){
        cout << "unknown tag family name (using the --family flag). " << family_name << " Quitting" << endl;
        exit(1);
    }

    apriltag_detector_t *td = apriltag_detector_create();
    apriltag_detector_add_family(td, tf);
    td->quad_decimate = decimate_factor;
    td->quad_sigma = blur;
    td->nthreads = number_threads;
    td->debug = debug;
    td->refine_edges = refine_edges;

    Mat frame, gray;

    if (video) {

        while (true) {
            cap >> frame;

            cvtColor(frame, gray, COLOR_BGR2GRAY);

            DetectAprilTagsInImages(frame, gray, td);

            imshow("Tag Detections", frame);
            if (waitKey(30) >= 0)
                break;
        }

    }   else {

        string filename;
        vector<string> image_files;
        ReadDirectory(input_dir, image_files);

        uint n = image_files.size();
        for (uint i = 0; i < n; i++ ){
            filename = input_dir + image_files[i];

            cout << "Filename " << filename << endl;

            frame = imread(filename.c_str() );
            cvtColor(frame, gray, COLOR_BGR2GRAY);

            DetectAprilTagsInImages(frame, gray, td);

            filename = output_dir + image_files[i];

            imwrite(filename.c_str(), frame);
        }

    }

    apriltag_detector_destroy(td);

    tag_destroy_functions[tag_index](tf);


    return 0;
}




void DetectAprilTagsInImages(Mat& frame, Mat& gray, apriltag_detector_t *td){

    // Make an image_u8_t header for the Mat data
    image_u8_t im = { .width = gray.cols,
            .height = gray.rows,
            .stride = gray.cols,
            .buf = gray.data
    };

    zarray_t *detections = apriltag_detector_detect(td, &im);
    cout << zarray_size(detections) << " tags detected" << endl;

    // Draw detection outlines
    for (int i = 0; i < zarray_size(detections); i++) {
        apriltag_detection_t *det;
        zarray_get(detections, i, &det);
        line(frame, Point(det->p[0][0], det->p[0][1]),
                Point(det->p[1][0], det->p[1][1]),
                Scalar(0, 0xff, 0), 2);
        line(frame, Point(det->p[0][0], det->p[0][1]),
                Point(det->p[3][0], det->p[3][1]),
                Scalar(0, 0, 0xff), 2);
        line(frame, Point(det->p[1][0], det->p[1][1]),
                Point(det->p[2][0], det->p[2][1]),
                Scalar(0xff, 0, 0), 2);
        line(frame, Point(det->p[2][0], det->p[2][1]),
                Point(det->p[3][0], det->p[3][1]),
                Scalar(0xff, 0, 0), 2);

        stringstream ss;
        ss << det->id;
        String text = ss.str();
        int fontface = FONT_HERSHEY_SCRIPT_SIMPLEX;
        double fontscale = 1.0;
        int baseline;
        Size textsize = getTextSize(text, fontface, fontscale, 2,
                &baseline);
        putText(frame, text, Point(det->c[0]-textsize.width/2,
                det->c[1]+textsize.height/2),
                fontface, fontscale, Scalar(0xff, 0x99, 0), 2);
    }
    apriltag_detections_destroy(detections);

}
