package com.bignerdranch.android.geoquiz;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.ActionBarActivity;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.TextView;
import android.widget.Toast;


public class QuizActivity extends ActionBarActivity {

    private static final String KEY_INDEX = "index";
    private static final String KEY_CHEATED = "cheated";
    private final TrueFalse[] mQuestionBank = new TrueFalse[]{
            new TrueFalse(R.string.question_oceans, true),
            new TrueFalse(R.string.question_mideast, false),
            new TrueFalse(R.string.question_africa, false),
            new TrueFalse(R.string.question_americas, true),
            new TrueFalse(R.string.question_asia, true)
    };
    private Button mTrueButton;
    private Button mFalseButton;
    private Button mCheatButton;
    private ImageButton mNextButton;
    private ImageButton mPrevButton;
    private TextView mQuestionTextView;
    private int mCurrentIndex;
    private boolean[] mCheated = {
            false,
            false,
            false,
            false,
            false
    };

    private void updateQuestion() {
        int question = mQuestionBank[mCurrentIndex].getQuestion();
        mQuestionTextView.setText(question);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_quiz);

        if (savedInstanceState == null) {
            mCurrentIndex = 0;
        } else {
            mCurrentIndex = savedInstanceState.getInt(KEY_INDEX, 0);
            mCheated = savedInstanceState.getBooleanArray(KEY_CHEATED);
        }

        mQuestionTextView = (TextView) findViewById(R.id.question_text_view);
        updateQuestion();
        mQuestionTextView.setOnClickListener(new NextListener(true));

        mTrueButton = (Button) findViewById(R.id.true_button);
        mTrueButton.setOnClickListener(new AnswerListener(true));

        mFalseButton = (Button) findViewById(R.id.false_button);
        mFalseButton.setOnClickListener(new AnswerListener(false));

        mCheatButton = (Button) findViewById(R.id.cheat_button);
        mCheatButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent i = new Intent(QuizActivity.this, CheatActivity.class);
                boolean answerIsTrue = mQuestionBank[mCurrentIndex].isTrueQuestion();
                i.putExtra(CheatActivity.EXTRA_ANSWER_IS_TRUE, answerIsTrue);
                startActivityForResult(i, 0);
            }
        });

        mNextButton = (ImageButton) findViewById(R.id.next_button);
        mNextButton.setOnClickListener(new NextListener(true));

        mPrevButton = (ImageButton) findViewById(R.id.prev_button);
        mPrevButton.setOnClickListener(new NextListener(false));
    }

    @Override
    protected void onSaveInstanceState(Bundle outState) {
        super.onSaveInstanceState(outState);
        outState.putInt(KEY_INDEX, mCurrentIndex);
        outState.putBooleanArray(KEY_CHEATED, mCheated);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (data == null) {
            return;
        }
        if (!mCheated[mCurrentIndex]) {
            boolean cheated = data.getBooleanExtra(CheatActivity.EXTRA_ANSWER_SHOWN, false);
            mCheated[mCurrentIndex] = cheated;
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_quiz, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    private class AnswerListener implements View.OnClickListener {

        private final boolean mAnswer;

        public AnswerListener(boolean answer) {
            mAnswer = answer;
        }

        private void checkAnswer() {
            int messageResId;

            if (mCheated[mCurrentIndex]) {
                messageResId = R.string.judgment_toast;
            } else {
                if (mAnswer == mQuestionBank[mCurrentIndex].isTrueQuestion()) {
                    messageResId = R.string.correct_toast;
                } else {
                    messageResId = R.string.incorrect_toast;
                }
            }

            Toast.makeText(QuizActivity.this, messageResId, Toast.LENGTH_SHORT).show();
        }

        @Override
        public void onClick(View v) {
            checkAnswer();
        }
    }

    private class NextListener implements View.OnClickListener {

        private final int mPower;

        public NextListener(boolean next) {
            mPower = next ? 0 : 1;
        }

        @Override
        public void onClick(View v) {
            mCurrentIndex = (int) (mQuestionBank.length * mPower + mCurrentIndex +
                    Math.pow(-1, mPower)) % mQuestionBank.length;
            updateQuestion();
        }
    }
}
