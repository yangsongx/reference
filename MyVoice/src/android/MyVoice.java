package szjy.com;

import org.apache.cordova.CordovaPlugin;
import org.apache.cordova.CallbackContext;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;
import java.util.HashMap;

import com.baidu.speech.EventManager;
import com.baidu.speech.EventManagerFactory;
import com.baidu.speech.EventListener;


/**
 * This class echoes a string called from JavaScript.
 */
public class MyVoice extends CordovaPlugin {
    public static final String TAG = "a22301";
    private EventManager mWpEventManager;
    private CallbackContext m_cb = null;

    @Override
    public boolean execute(String action, JSONArray args, CallbackContext callbackContext) throws JSONException {
        if (action.equals("coolMethod")) {
            android.util.Log.e(TAG, "Mark line of API calling!");
            String message = args.getString(0);
            //this.coolMethod(message, callbackContext);
            this.startWaitVoiceCmd(message, callbackContext);
            return true;
        }
        return false;
    }

    private void coolMethod(String message, CallbackContext callbackContext) {
        if (message != null && message.length() > 0) {
            callbackContext.success(message);
        } else {
            callbackContext.error("Expected one non-empty string argument.");
        }
    }

    private void startWaitVoiceCmd(String message, CallbackContext callbackContext) {
        m_cb = callbackContext;

        android.util.Log.e(TAG, "In my java function...");
        mWpEventManager = EventManagerFactory.create(cordova.getActivity(), "wp");

        mWpEventManager.registerListener(new EventListener() {
            @Override
            public void onEvent(String name, String params, byte[] data, int offset, int length){

                String val;

                android.util.Log.e(TAG, String.format("event: name=%s, params=%s", name, params));
                try{
                    JSONObject json = new JSONObject(params);
                    if("wp.data".equals(name)) {
                        android.util.Log.e(TAG, "BINGO~~, you say it!");
                        val = "WAKUP!";
                        notifyResult(val, true); // GOOD, tell upper layer...
                    } else if ("wp.exit".equals(name)) {
                        android.util.Log.e(TAG, "STOPPED, you need restart again");
                        val = "STOPPED, need restart";
                        notifyResult(val, false);
                    }
                } catch (JSONException e) {
                    android.util.Log.e(TAG, "got JSON Exception...");
                    val = "Exception found";
                    notifyResult(val, false);
                }
            }
        });

        HashMap params = new HashMap();
        params.put("kws-file", "assets:///WakeUp.bin");
        mWpEventManager.send("wp.start", new JSONObject(params).toString(), null, 0, 0);
        android.util.Log.e(TAG, "-->tracking started...");

        //callbackContext.success("COOL");
    }

    private void notifyResult(String data, boolean goodNews){
        if (goodNews) {
            m_cb.success(data);
        } else {
            m_cb.error(data);
        }
    }
}
