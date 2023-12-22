package org.techtown.doitmission03;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;

public class MainActivity extends AppCompatActivity {
    ImageView imageView01;
    ImageView imageView02;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        imageView01 = findViewById(R.id.imageView01);
        imageView02 = findViewById(R.id.imageView02);

        Button btnUp = findViewById(R.id.btnUp);
        btnUp.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                moveImageUp();
            }
        });

        Button btnDown = findViewById(R.id.btnDown);
        btnDown.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                moveImageDown();
            }
        });
        moveImageUp();
    }
    private void moveImageUp(){
        imageView01.setImageResource(R.drawable.dream01);
        imageView02.setImageResource(0);

        imageView01.invalidate();
        imageView02.invalidate();
    }

    private void moveImageDown(){
        imageView01.setImageResource(0);
        imageView02.setImageResource(R.drawable.dream01);

        imageView01.invalidate();
        imageView02.invalidate();
    }
}