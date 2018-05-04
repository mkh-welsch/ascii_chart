/* Signal Epowering Technology   
                       
presents

 █████╗ ███████╗ ██████╗██╗██╗     ██████╗██╗  ██╗ █████╗ ██████╗ ████████╗
██╔══██╗██╔════╝██╔════╝██║██║    ██╔════╝██║  ██║██╔══██╗██╔══██╗╚══██╔══╝
███████║███████╗██║     ██║██║    ██║     ███████║███████║██████╔╝   ██║   
██╔══██║╚════██║██║     ██║██║    ██║     ██╔══██║██╔══██║██╔══██╗   ██║   
██║  ██║███████║╚██████╗██║██║    ╚██████╗██║  ██║██║  ██║██║  ██║   ██║   
╚═╝  ╚═╝╚══════╝ ╚═════╝╚═╝╚═╝     ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝                                                                             
                                                    Licensed under MPL 2.0. 
                                                   Michael Welsch (c) 2018.
                                                                                                   
a simple function to plot a line to console.

This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.
 
usage: 
ascii_chart::plot(std::vector<T> s0);
ascii_chart::plot(std::vector<T> s0, int heigth, int width);
 
 */

/*** Usage with default L2 metric for abitrary stl containers. ***/

#ifndef ASCII_CHART_H
#define ASCII_CHART_H

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip> // setprecision
#include <sstream> // stringstream

namespace ascii_chart
{

template <typename Container>
void plot(Container series, int h = 16, int w = 100)
{
    typedef typename Container::value_type T;

    T min = series[0];
    T max = series[0];

    for (int i = 1; i < series.size(); i++)
    {
        min = std::min(min, series[i]);
        max = std::max(max, series[i]);
    }

    T range = std::abs(max - min);

    int offset = 3;
    std::string padding = "           ";

    int height = h;
    T ratio = T(height) / range;
    int min2 = int(std::round(min * ratio));
    int max2 = int(std::round(max * ratio));
    int rows = int(std::abs(max2 - min2));
    int width = w;

    std::vector<std::vector<std::string>> result(rows + 1, std::vector<std::string>(width));

    for (int i = 0; i <= rows; i++)
    {
        for (int j = 0; j < width; j++)
        {
            result[i][j].append(" ");
        }
    }

    // axis + labels
    for (int y = min2; y <= max2; ++y)
    {
        float val = float(max) - (float(y) - float(min2)) * float(range) / float(rows);
        std::stringstream stream;
        stream << std::fixed << std::right << std::setw(padding.size()) << std::setprecision(2) << val;
        std::string slabel = stream.str();

        std::string label = std::string(slabel);
        for (int i = 0; i < result[y - min2][std::max(int(offset) - int(slabel.size()), int(0))].size(); ++i)
        {
            result[y - min2][std::max(int(offset) - int(slabel.size()), int(0))].pop_back();
        }
        result[y - min2][std::max(int(offset) - int(slabel.size()), int(0))].append(slabel);
        while (result[y - min2][std::max(int(offset) - int(slabel.size()), int(0))].size() < padding.size())
        {
            result[y - min2][std::max(int(offset) - int(slabel.size()), int(0))].append(" ");
        }

        result[y - min2][offset - 1] = (y == 0) ? "┼" : "┤";
    }

    int y0 = int(std::round(series[0] * ratio)) - min2;

    result[rows - y0][offset - 1].pop_back();
    result[rows - y0][offset - 1].pop_back();
    result[rows - y0][offset - 1].pop_back();
    result[rows - y0][offset - 1].pop_back();
    result[rows - y0][offset - 1].append("┼"); // first value

    for (int x = 0; x < series.size() - 1; x++)
    { // plot the line
        int y0 = int(std::round(series[x + 0] * ratio)) - min2;
        int y1 = int(std::round(series[x + 1] * ratio)) - min2;
        if (y0 == y1)
        {
            result[rows - y0][x + offset] = "─";
        }
        else
        {
            result[rows - y1][x + offset] = (y0 > y1) ? "└" : "┌";
            result[rows - y0][x + offset] = (y0 > y1) ? "┐" : "┘";

            int from = std::min(y0, y1);
            int to = std::max(y0, y1);
            for (int y = from + 1; y < to; y++)
            {
                result[rows - y][x + offset] = "│";
            }
        }
    }

    for (int i = 0; i < result.size(); ++i)
    {
        for (int j = 0; j < result[i].size(); ++j)
        {
            std::cout << result[i][j];
        }
        std::cout << std::endl;
    }
}

} // end namespace
#endif