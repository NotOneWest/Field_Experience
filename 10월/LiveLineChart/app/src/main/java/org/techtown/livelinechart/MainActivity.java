package org.techtown.livelinechart;
import androidx.appcompat.app.AppCompatActivity;

import android.graphics.Color;
import android.os.Bundle;
import android.util.Log;

import com.github.mikephil.charting.charts.LineChart;
import com.github.mikephil.charting.components.Legend;
import com.github.mikephil.charting.components.LegendEntry;
import com.github.mikephil.charting.components.XAxis;
import com.github.mikephil.charting.components.YAxis;
import com.github.mikephil.charting.data.Entry;
import com.github.mikephil.charting.data.LineData;
import com.github.mikephil.charting.data.LineDataSet;
import com.github.mikephil.charting.interfaces.datasets.ILineDataSet;

import java.util.ArrayList;
import java.util.List;

public class MainActivity extends AppCompatActivity {

    private LineChart chart;
    private Thread thread;
    private float[][] speeds =
            {
                    {
                            5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F,
                            5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F,
                            5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F,
                            5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F,
                            5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F,
                            5.0F, 5.0F, 5.0F, 5.0F, 6.0F, 6.0F, 6.0F, 6.0F, 6.0F, 6.0F, 7.0F, 7.0F, 7.0F, 7.0F,
                            7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F,
                            7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F,
                            7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 8.0F, 8.0F, 8.0F, 8.0F, 8.0F, 8.0F, 8.0F, 8.0F,
                            9.0F, 9.0F, 9.0F, 9.0F, 9.0F, 9.0F, 9.0F, 9.0F, 9.0F, 9.0F, 9.0F, 9.0F, 9.0F, 9.0F,
                            9.0F, 9.0F, 9.0F, 9.0F, 9.0F, 9.0F, 9.0F, 9.0F, 9.0F, 9.0F, 9.0F, 9.0F, 9.0F, 9.0F,
                            9.0F, 9.0F, 9.0F, 9.0F, 9.0F, 9.0F, 9.0F, 9.0F, 9.0F, 9.0F, 9.0F, 9.0F, 11.0F, 11.0F,
                            11.0F, 11.0F, 11.0F, 11.0F, 11.0F, 11.0F, 11.0F, 11.0F, 11.0F, 11.0F, 11.0F, 11.0F,
                            11.0F, 11.0F, 11.0F, 11.0F, 11.0F, 11.0F, 11.0F, 11.0F, 11.0F, 11.0F, 11.0F, 11.0F,
                            11.0F, 11.0F, 11.0F, 11.0F, 11.0F, 11.0F, 8.0F, 8.0F, 8.0F, 8.0F, 8.0F, 8.0F, 8.0F,
                            8.0F, 8.0F, 8.0F, 8.0F, 8.0F, 8.0F, 8.0F, 8.0F, 8.0F, 8.0F, 8.0F, 8.0F, 8.0F, 8.0F,
                            8.0F, 8.0F, 8.0F, 8.0F, 8.0F, 8.0F, 8.0F, 8.0F, 8.0F, 8.0F, 8.0F, 8.0F, 8.0F, 8.0F,
                            8.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 0.0F, 0.0F,
                    },
                    {
                            6.0F, 6.0F, 6.0F, 6.0F, 6.0F, 6.0F, 6.0F, 7.0F, 7.0F, 6.0F, 7.0F, 7.0F, 7.0F, 7.0F,
                            7.0F, 7.0F, 7.0F, 8.0F, 8.0F, 8.0F, 8.0F, 8.0F, 8.0F, 8.0F, 8.0F, 8.0F, 8.0F, 8.0F,
                            8.0F, 8.0F, 8.0F, 8.0F, 8.0F, 8.0F, 9.0F, 9.0F, 9.0F, 9.0F, 9.0F, 9.0F, 9.0F, 9.0F,
                            9.0F, 9.0F, 9.0F, 9.0F, 9.0F, 9.0F, 9.0F, 9.0F, 9.0F, 9.0F, 9.0F, 9.0F, 9.0F, 9.0F,
                            9.0F, 9.0F, 9.0F, 9.0F, 9.0F, 9.0F, 9.0F, 9.0F, 9.0F, 9.0F, 11.0F, 11.0F, 11.0F,
                            11.0F, 11.0F, 11.0F, 11.0F, 11.0F, 11.0F, 11.0F, 11.0F, 11.0F, 11.0F, 11.0F, 11.0F,
                            11.0F, 11.0F, 11.0F, 11.0F, 11.0F, 11.0F, 11.0F, 11.0F, 11.0F, 11.0F, 11.0F, 11.0F,
                            11.0F, 11.0F, 0.0F, 0.0F
                    },
                    {
                            6.0F, 6.0F, 6.0F, 6.0F, 6.0F, 6.0F, 6.0F, 6.0F, 6.0F, 6.0F, 6.0F, 6.0F, 6.0F, 6.0F,
                            6.0F, 6.0F, 6.0F, 6.0F, 6.0F, 6.0F, 6.0F, 6.0F, 6.0F, 6.0F, 6.0F, 6.0F, 0.0F, 0.0F,
                            6.0F, 6.0F, 6.0F, 6.0F, 6.0F, 6.0F, 6.0F, 6.0F, 6.0F, 6.0F, 6.0F, 6.0F, 6.0F, 6.0F,
                            6.0F, 6.0F, 10.0F, 10.0F, 10.0F, 10.0F, 10.0F, 10.0F, 10.0F, 10.0F, 6.0F, 6.0F,
                            6.0F, 6.0F, 6.0F, 6.0F, 6.0F, 6.0F, 6.0F, 6.0F, 6.0F, 6.0F, 10.0F, 10.0F, 10.0F,
                            10.0F, 10.0F, 10.0F, 6.0F, 6.0F, 6.0F, 6.0F, 6.0F, 6.0F, 10.0F, 10.0F, 10.0F, 10.0F,
                            10.0F, 10.0F, 10.0F, 10.0F, 10.0F, 10.0F, 10.0F, 10.0F, 6.0F, 6.0F, 6.0F, 6.0F, 6.0F,
                            6.0F, 6.0F, 6.0F, 10.0F, 10.0F, 10.0F, 10.0F, 10.0F, 10.0F, 10.0F, 10.0F, 10.0F, 10.0F,
                            10.0F, 10.0F, 10.0F, 10.0F, 10.0F, 10.0F, 4.0F, 4.0F, 4.0F, 0.0F, 0.0F
                    },
                    {
                            5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F,
                            5.0F, 5.0F, 6.5F, 6.5F, 6.5F, 6.5F, 6.5F, 6.5F, 6.5F, 6.5F, 6.5F, 6.5F, 6.5F, 6.5F,
                            6.5F, 6.5F, 8.0F, 8.0F, 8.0F, 8.0F, 8.0F, 8.0F, 8.0F, 8.0F, 10.0F, 10.0F, 10.0F,
                            10.0F, 11.0F, 11.0F, 11.0F, 11.0F, 11.0F, 11.0F, 11.0F, 11.0F, 11.0F, 11.0F, 11.0F,
                            11.0F, 11.0F, 11.0F, 11.0F, 11.0F, 11.0F, 0.0F, 0.0F
                    },
                    {
                            5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F,
                            5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 7.0F, 7.0F,
                            7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F,
                            7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F,
                            7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F,
                            7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F,
                            7.0F, 7.0F, 8.0F, 8.0F, 8.0F, 8.0F, 8.0F, 8.0F, 8.0F, 8.0F, 8.0F, 8.0F, 8.0F, 8.0F,
                            8.0F, 8.0F, 8.0F, 8.0F, 8.0F, 8.0F, 8.0F, 8.0F, 8.0F, 8.0F, 8.0F, 8.0F, 8.0F, 8.0F,
                            8.0F, 8.0F, 8.0F, 8.0F, 8.0F, 8.0F, 8.0F, 0.0F, 0.0F
                    },
                    {
                            5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F,
                            5.0F, 5.0F, 6.5F, 6.5F, 6.5F, 6.5F, 6.5F, 6.5F, 6.5F, 6.5F, 6.5F, 6.5F, 6.5F, 6.5F,
                            6.5F, 6.5F, 6.5F, 6.5F, 6.5F, 6.5F, 6.5F, 6.5F, 6.5F, 6.5F, 6.5F, 6.5F, 6.5F, 6.5F,
                            6.5F, 6.5F, 6.5F, 6.5F, 6.5F, 6.5F, 6.5F, 6.5F, 6.5F, 6.5F, 6.5F, 6.5F, 7.5F, 7.5F,
                            7.5F, 7.5F, 7.5F, 7.5F, 7.5F, 7.5F, 7.5F, 7.5F, 7.5F, 7.5F, 7.5F, 7.5F, 7.5F, 7.5F,
                            7.5F, 7.5F, 7.5F, 7.5F, 7.5F, 7.5F, 7.5F, 7.5F, 7.5F, 0.0F, 0.0F

                    },
                    {
                            5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 7.0F, 7.0F, 7.0F, 7.0F,
                            7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 9.0F, 9.0F, 9.0F, 9.0F, 9.0F, 9.0F,
                            9.0F, 9.0F, 9.0F, 9.0F, 11.0F, 11.0F, 11.0F, 11.0F, 11.0F, 3.0F, 3.0F, 0.0F, 0.0F
                    },
                    {
                            5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F,
                            5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 0.0F, 0.0F, 0.0F, 0.0F, 7.0F, 7.0F, 7.0F, 7.0F,
                            7.0F, 7.0F, 7.0F, 7.0F, 5.0F, 5.0F, 5.0F, 5.0F, 9.0F, 9.0F, 9.0F, 9.0F, 5.0F, 5.0F,
                            5.0F, 5.0F, 12.0F, 12.0F, 5.0F, 5.0F, 5.0F, 5.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F,
                            7.0F, 7.0F, 5.0F, 5.0F, 5.0F, 5.0F, 9.0F, 9.0F, 9.0F, 9.0F, 5.0F, 5.0F, 5.0F, 5.0F,
                            12.0F, 12.0F, 5.0F, 5.0F, 5.0F, 5.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F, 7.0F,
                            5.0F, 5.0F, 5.0F, 5.0F, 9.0F, 9.0F, 9.0F, 9.0F, 5.0F, 5.0F, 5.0F, 5.0F, 12.0F, 12.0F,
                            5.0F, 0.0F, 0.0F
                    }
            };

    private float[][] slopes =
            {
                    {
                            0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
                            0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
                            0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 2.0F, 2.0F, 2.0F, 2.0F,
                            2.0F, 2.0F, 2.0F, 2.0F, 2.0F, 2.0F, 2.0F, 2.0F, 2.0F, 2.0F, 2.0F, 2.0F, 2.0F, 2.0F,
                            2.0F, 2.0F, 2.0F, 2.0F, 2.0F, 2.0F, 2.0F, 2.0F, 2.0F, 2.0F, 3.0F, 3.0F, 3.0F, 3.0F,
                            3.0F, 3.0F, 3.0F, 3.0F, 2.5F, 2.5F, 2.5F, 2.5F, 2.5F, 2.5F, 1.0F, 1.0F, 1.0F, 1.0F,
                            1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F,
                            1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 3.0F, 3.0F,
                            3.0F, 3.0F, 5.0F, 5.0F, 6.0F, 6.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F,
                            1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F,
                            1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F,
                            1.0F, 1.0F, 2.0F, 2.0F, 2.5F, 2.5F, 2.5F, 2.5F, 2.5F, 2.5F, 2.5F, 2.5F, 0.0F, 0.0F,
                            0.0F, 0.0F, 0.0F, 0.0F, 4.0F, 4.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
                            2.0F, 2.0F, 2.0F, 2.0F, 4.0F, 4.0F, 4.0F, 4.0F, 6.0F, 6.0F, 8.0F, 1.0F, 1.0F, 1.0F,
                            1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F,
                            1.0F, 1.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
                            0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
                            0.0F, 0.0F, 0.0F
                    },
                    {
                            0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
                            0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
                            0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
                            1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 2.5F, 2.5F, 2.5F, 2.5F,
                            2.5F, 2.5F, 2.5F, 2.5F, 2.5F, 2.5F, 2.5F, 2.5F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F,
                            1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 4.0F, 4.0F,
                            4.0F, 4.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F
                    },
                    {
                            0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
                            0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
                            0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
                            0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 1.0F,
                            1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F,
                            1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F,
                            1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F,
                            1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F,
                            0.0F, 0.0F, 0.0F, 0.0F, 0.0F
                    },
                    {
                            0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
                            1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F,
                            1.0F, 1.0F, 2.0F, 2.0F, 2.0F, 2.0F, 2.0F, 2.0F, 2.0F, 2.0F, 2.0F, 2.0F, 0.0F, 0.0F,
                            0.0F, 0.0F, 0.0F, 0.0F, 4.0F, 4.0F, 4.0F, 4.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F,
                            1.0F, 1.0F, 1.0F, 0.0F, 0.0F
                    },
                    {
                            0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
                            0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 1.0F, 1.0F, 0.0F, 0.0F, 0.0F, 0.0F,
                            0.0F, 0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 1.0F, 1.0F, 2.0F, 2.0F, 2.0F, 2.0F, 0.0F, 0.0F,
                            0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 1.0F, 1.0F, 2.0F, 2.0F, 2.0F, 2.0F,
                            3.0F, 3.0F, 3.0F, 3.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 1.0F, 1.0F,
                            1.0F, 1.0F, 2.0F, 2.0F, 2.0F, 2.0F, 3.0F, 3.0F, 3.0F, 3.0F, 4.0F, 4.0F, 4.0F, 4.0F,
                            0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 1.0F, 1.0F, 2.0F, 2.0F,
                            2.0F, 2.0F, 3.0F, 3.0F, 3.0F, 3.0F, 4.0F, 4.0F, 4.0F, 4.0F, 5.0F, 5.0F, 5.0F, 5.0F,
                            0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F
                    },
                    {
                            0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 1.0F, 1.0F, 2.0F, 2.0F,
                            2.0F, 2.0F, 0.0F, 0.0F, 0.0F, 0.0F, 4.0F, 4.0F, 4.0F, 4.0F, 0.0F, 0.0F, 0.0F, 0.0F,
                            2.0F, 2.0F, 2.0F, 2.0F, 1.0F, 1.0F, 1.0F, 1.0F, 3.0F, 3.0F, 3.0F, 3.0F, 2.0F, 2.0F,
                            2.0F, 2.0F, 5.0F, 5.0F, 5.0F, 5.0F, 3.0F, 3.0F, 3.0F, 3.0F, 0.0F, 0.0F, 0.0F, 0.0F,
                            1.0F, 0.0F, 0.0F, 0.0F, 6.0F, 6.0F, 6.0F, 6.0F, 2.0F, 2.0F, 2.0F, 2.0F, 3.0F, 3.0F,
                            3.0F, 3.0F, 2.0F, 2.0F, 2.0F, 2.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F
                    },
                    {
                            0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
                            0.0F, 0.0F, 0.0F, 0.0F, 4.0F, 4.0F, 4.0F, 4.0F, 2.0F, 2.0F, 2.0F, 2.0F, 2.0F, 2.0F,
                            2.0F, 2.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 0.0F, 0.0F, 0.0F, 0.0F
                    },
                    {
                            0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
                            0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 2.0F, 2.0F,
                            3.0F, 3.0F, 3.0F, 3.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 2.0F, 2.0F, 0.0F, 0.0F,
                            0.0F, 0.0F, 1.0F, 1.0F, 0.0F, 0.0F, 0.0F, 0.0F, 3.0F, 3.0F, 3.0F, 3.0F, 3.0F, 3.0F,
                            3.0F, 3.0F, 0.0F, 0.0F, 0.0F, 0.0F, 2.0F, 2.0F, 2.0F, 2.0F, 0.0F, 0.0F, 0.0F, 0.0F,
                            1.0F, 1.0F, 0.0F, 0.0F, 0.0F, 0.0F, 3.0F, 3.0F, 3.0F, 3.0F, 3.0F, 3.0F, 3.0F, 3.0F,
                            0.0F, 0.0F, 0.0F, 0.0F, 2.0F, 2.0F, 2.0F, 2.0F, 0.0F, 0.0F, 0.0F, 0.0F, 1.0F, 1.0F,
                            0.0F, 0.0F, 0.0F
                    }
            };
    private int index = 0;
    private int position = 7;
    private List<Entry> speed_list = new ArrayList<Entry>();
    private List<Entry> slope_list = new ArrayList<Entry>();
    List<ILineDataSet> dataSets = new ArrayList<ILineDataSet>();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        chart = (LineChart) findViewById(R.id.chart);
        chart.getDescription().setEnabled(false);
        chart.setScaleEnabled(false);

        LegendEntry[] legendEntries = new LegendEntry[2];
        int[] colorArray = {Color.parseColor("#FF02AC4A"), Color.parseColor("#FF56B0C9")};
        String[] legendName = {"Speed", "Slope"};
        for(int i=0; i<2; i++){
            LegendEntry entry = new LegendEntry();
            entry.formColor = colorArray[i];
            entry.label = legendName[i];
            legendEntries[i] = entry;
        }
        chart.getLegend().setTextSize(12);
        chart.getLegend().setTextColor(Color.WHITE);
        chart.getLegend().setCustom(legendEntries);

        chart.getXAxis().setPosition(XAxis.XAxisPosition.BOTTOM);
        chart.getXAxis().setAxisMaximum(speeds[position].length);
        chart.getXAxis().setDrawGridLines(false);
        chart.getXAxis().setDrawLabels(false);
        chart.getXAxis().setAxisMinimum(0f);
        chart.getXAxis().setEnabled(false);

        chart.getAxisLeft().setDrawGridLines(false);
        chart.getAxisLeft().setAxisMinimum(-0.1f);
        chart.getAxisLeft().setAxisMaximum(13);
        chart.getAxisRight().setEnabled(false);
        chart.getAxisLeft().setEnabled(false);

        chart.animateXY(2000, 2000);
        chart.invalidate();

        addShadow();

        feedMultiple();
    }

    private void addShadow(){
        List<Entry> speed_list_all = new ArrayList<Entry>();
        List<Entry> slope_list_all = new ArrayList<Entry>();

        for(int i=0; i<speeds[position].length; i++){
            speed_list_all.add(new Entry(i, speeds[position][i]));
            slope_list_all.add(new Entry(i, slopes[position][i]));
        }

        LineDataSet speed_set_all = new LineDataSet(speed_list_all, null);
        LineDataSet slope_set_all = new LineDataSet(slope_list_all, null);

        speed_set_all.setLineWidth(2); slope_set_all.setLineWidth(2);
        speed_set_all.setDrawValues(false); slope_set_all.setDrawValues(false);
        speed_set_all.setDrawCircles(false); slope_set_all.setDrawCircles(false);
        speed_set_all.setDrawCircleHole(false); slope_set_all.setDrawCircleHole(false);
        speed_set_all.setColor(Color.GRAY); slope_set_all.setColor(Color.GRAY);
        speed_set_all.setMode(LineDataSet.Mode.HORIZONTAL_BEZIER); slope_set_all.setMode(LineDataSet.Mode.HORIZONTAL_BEZIER);
        speed_set_all.setAxisDependency(YAxis.AxisDependency.LEFT); slope_set_all.setAxisDependency(YAxis.AxisDependency.LEFT);

        dataSets.add(speed_set_all); dataSets.add(slope_set_all);
    }

    private void addEntry() {
        if(index < speeds[position].length){
            speed_list.add(new Entry(index, speeds[position][index]));
            slope_list.add(new Entry(index, slopes[position][index]));
            index++;

            LineDataSet speed_set = new LineDataSet(speed_list, "Speed");
            LineDataSet slope_set = new LineDataSet(slope_list, "Slope");

            speed_set.setLineWidth(2); slope_set.setLineWidth(2);
            speed_set.setDrawValues(false); slope_set.setDrawValues(false);
            speed_set.setDrawCircles(false); slope_set.setDrawCircles(false);
            speed_set.setDrawCircleHole(false); slope_set.setDrawCircleHole(false);
            speed_set.setColor(Color.parseColor("#FF02AC4A")); slope_set.setColor(Color.parseColor("#FF56B0C9"));
            speed_set.setMode(LineDataSet.Mode.HORIZONTAL_BEZIER); slope_set.setMode(LineDataSet.Mode.HORIZONTAL_BEZIER);
            speed_set.setAxisDependency(YAxis.AxisDependency.LEFT); slope_set.setAxisDependency(YAxis.AxisDependency.LEFT);

            dataSets.add(speed_set); dataSets.add(slope_set);

            LineData data = new LineData(dataSets);
            chart.setData(data);

            chart.notifyDataSetChanged();
            chart.moveViewToX(data.getEntryCount());
            chart.setVisibleXRangeMaximum(speeds[position].length);

            chart.invalidate();
        }
    }

    private void feedMultiple() {
        if (thread != null)
            thread.interrupt();

        if(index == speeds[position].length)
            thread.interrupt();

        final Runnable runnable = new Runnable() {
            @Override
            public void run() {
                if(index < speeds[position].length)
                    addEntry();
            }
        };

        thread = new Thread(new Runnable() {
            @Override
            public void run() {
                int flag = 0;
                while (true) {
                    runOnUiThread(runnable);
                    try {
                        if(flag == 0) flag++;
                        else Thread.sleep(2000);
                    } catch (InterruptedException ie) {
                        ie.printStackTrace();
                    }
                }
            }
        });
        thread.start();
    }

    @Override
    protected void onPause() {
        super.onPause();
        if (thread != null)
            thread.interrupt();
    }
}