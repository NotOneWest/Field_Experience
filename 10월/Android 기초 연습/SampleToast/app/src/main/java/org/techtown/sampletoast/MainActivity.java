package org.techtown.sampletoast;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import android.app.ProgressDialog;
import android.content.DialogInterface;
import android.os.Bundle;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.Button;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

import com.google.android.material.snackbar.Snackbar;

public class MainActivity extends AppCompatActivity {
    TextView textView;
    ProgressDialog dialog;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        textView = findViewById(R.id.textView);

        Button button3 = findViewById(R.id.button3);
        button3.setOnClickListener(view -> showMSG());

        ProgressBar progressBar = findViewById(R.id.progressBar);
        progressBar.setIndeterminate(false);
        progressBar.setProgress(80);
        Button button4 = findViewById(R.id.button4);
        button4.setOnClickListener(view -> {
            dialog = new ProgressDialog(MainActivity.this);
            dialog.setProgressStyle(ProgressDialog.STYLE_SPINNER);
            dialog.setMessage("데이터를 확인하는 중입니다.");
            dialog.show();
        });

        Button button5 = findViewById(R.id.button5);
        button5.setOnClickListener(view -> {
            if(dialog != null){
                dialog.dismiss();
            }
        });
    }

    private void showMSG(){
        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        builder.setTitle("안내");
        builder.setMessage("종료하시겠습니까?");
        builder.setIcon(android.R.drawable.ic_dialog_alert);

        builder.setPositiveButton("예", (dialogInterface, i) -> {
            String msg = "예 버튼이 눌렸습니다. ";
            textView.setText(msg);
        });

        builder.setNeutralButton("취소", (dialogInterface, i) -> {
            String msg = "취소 버튼이 눌렸습니다. ";
            textView.setText(msg);
        });

        builder.setNegativeButton("아니오", (dialogInterface, i) -> {
            String msg = "아니오 버튼이 눌렸습니다. ";
            textView.setText(msg);
        });
        AlertDialog alertDialog = builder.create();
        alertDialog.show();
    }

    public void onBtn1Cliked(View v){
        LayoutInflater inflater = getLayoutInflater();

        View layout = inflater.inflate(R.layout.toastborder, findViewById(R.id.toast_layout_root));

        TextView text = layout.findViewById(R.id.text);

        Toast toast = new Toast(this);
        text.setText("모양 바꾼 토스트");
        toast.setGravity(Gravity.CENTER, 0, -100);
        toast.setDuration(Toast.LENGTH_LONG);
        toast.setView(layout);
        toast.show();
    }

    public void onBtn2Cliked(View v){
        Snackbar.make(v, "스낵바입니다.", Snackbar.LENGTH_LONG).show();
    }

}