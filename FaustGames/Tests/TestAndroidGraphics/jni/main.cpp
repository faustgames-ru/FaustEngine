/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

//BEGIN_INCLUDE(all)
#include <jni.h>
#include <errno.h>

#include <EGL/egl.h>
#include <GLES/gl.h>

#include <android/sensor.h>
#include <android/log.h>
#include "../native_app_glue/android_native_app_glue.h"
#include "llge.h"
#include "vector"
#include <stdlib.h>
#include <string>
#include <sstream>
#include <stdio.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))

/**
 * Our saved state data.
 */
struct saved_state {
    float angle;
    int32_t x;
    int32_t y;
};
/**
 * Shared state for our app.
 */
struct engine {
    struct android_app* app;

    ASensorManager* sensorManager;
    const ASensor* accelerometerSensor;
    ASensorEventQueue* sensorEventQueue;

    int animating;
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
    int32_t width;
    int32_t height;
    struct saved_state state;
};

/**
 * Initialize an EGL context for the current display.
 */

llge::ITestRenderSystem *_renderSystem;
llge::IFactory *_factory;

#define SIZE 2
#define SIZE1 18

static double now_ms(void) {

	struct timespec res;
	clock_gettime(CLOCK_REALTIME, &res);
	return 1000.0 * res.tv_sec + (double)res.tv_nsec / 1e6;

}

int64_t getTimeNsec() 
{
	return (int64_t)now_ms();
	//struct timespec now;
	//clock_gettime(CLOCK_MONOTONIC, &now);
	//return (int64_t)now.tv_sec * 1000000000LL + now.tv_nsec;
}

template <typename T>
std::string to_string(T value)
{
	std::ostringstream os;
	os << value;
	return os.str();
}


class GlobalConstants
{
public:
	static const int ObjectsLimit = 8 * 1024 * 1024;
};

namespace core
{
	template<class T, int size>
	class FixedPool
	{
	private:
		static T *_hollows[size];
		static int _count;
		static int _limit;
		static int _hollowsCount;
		static T * _root;

	public:
		static void initialize()
		{
			_root = (T*)malloc(size*sizeof(T));
		}

		static inline int getCount(){ return _count; }
		static inline int getIndex(T *value)
		{
			return value - _root;
		}
		static inline T * create()
		{
			if (_hollowsCount > 0)
			{
				T *place = _hollows[_hollowsCount];
				T *instance = new (place)T();
				--_hollowsCount;
				_count++;
				return instance;
			}
			else
			{
#if _DEBUG
				if (_count >= _limit)
				{
					//throw std::exception("pool overflow");
				}
#endif		
				T *place = _root + _count;
				T *instance = new (place)T();
				_count++;
				return instance;
			}
		}
		static inline T * getByIndex(int id)
		{
			return _root + id;
		}
		static inline void release(T * value)
		{
			_hollows[_hollowsCount] = value;
			++_hollowsCount;
			value->~T();
		}
	};

	template <class T, int size>
	T * FixedPool<T, size>::_root(0);

	template <class T, int size>
	T * FixedPool<T, size>::_hollows[size];

	template <class T, int size>
	int FixedPool<T, size>::_count(0);

	template <class T, int size>
	int FixedPool<T, size>::_limit(size);

	template <class T, int size>
	int FixedPool<T, size>::_hollowsCount(0);

}

struct ComponentTransform2d
{
	float LocalX;
	float LocalY;
	float LocalZ;
	float LocalPivotX;
	float LocalPivotY;
	float LocalRotation;
	float LocalScale;

	float GlobalX;
	float GlobalY;
	float GlobalZ;
	float GlobalPivotX;
	float GlobalPivotY;
	float GlobalRotation;
	float GlobalScale;

	ComponentTransform2d * Parent;
	ComponentTransform2d * NextNodeId;
	ComponentTransform2d * FirtsChildId;
};

class SystemTransform2dNode
{
public:
	SystemTransform2dNode * Parent;
	ComponentTransform2d *Item;
	std::vector<SystemTransform2dNode *> Childs;

	SystemTransform2dNode()
	{
		Parent = 0;
		Item = Pool::create();

	}
	typedef core::FixedPool<ComponentTransform2d, GlobalConstants::ObjectsLimit> Pool;
	typedef core::FixedPool<SystemTransform2dNode, GlobalConstants::ObjectsLimit> ExPool;
};

class SystemTransform2dLink
{
public:
	SystemTransform2dNode *Root;
	float r;
	inline float createRand()
	{
		r += 0.1f;
		return r;
	}

	SystemTransform2dNode * createRoot()
	{
		Root = new SystemTransform2dNode();
		Root->Parent = 0;
		r = 0;
		return Root;
	}

	float randomSwap[128];
	int randomIndex;

	inline float getRand()
	{
		++randomIndex;
		return randomSwap[randomIndex % 128];
	}

	inline void randomize(ComponentTransform2d *node)
	{
		node->LocalX = node->GlobalX = getRand();
		node->LocalX = node->GlobalY = getRand();
		node->LocalX = node->GlobalZ = getRand();

		node->LocalRotation = node->GlobalRotation = getRand();
		node->LocalScale = node->GlobalScale = getRand();
	}

	static int iterations;


	void create(SystemTransform2dNode *root, int depth)
	{
		iterations++;
		randomize(root->Item); 
		if (depth == 0) return;
		for (int i = 0; i < SIZE; i++)
		{
			SystemTransform2dNode * child = new SystemTransform2dNode();
			root->Childs.push_back(child);
			create(child, depth - 1);
		}
	}

	inline void transform(ComponentTransform2d *node, ComponentTransform2d *parent)
	{
		node->GlobalX = node->LocalX + parent->GlobalX;
		node->GlobalY = node->LocalY + parent->GlobalY;
		node->GlobalZ = node->LocalZ + parent->GlobalZ;

		node->GlobalRotation = node->LocalRotation + parent->GlobalRotation;
		node->GlobalScale = node->LocalScale * parent->GlobalScale;
	}


	void parse(SystemTransform2dNode *root)
	{
		//iterations++;
		int size = root->Childs.size();
		ComponentTransform2d *node; 
		for (int i = 0; i < size; i++)
		{
			//transform(root->Childs[i]->Item, root->Item);
			node = root->Childs[i]->Item;
			node->GlobalX = node->LocalX + root->Item->GlobalX;
			node->GlobalY = node->LocalY + root->Item->GlobalY;
			node->GlobalZ = node->LocalZ + root->Item->GlobalZ;

			node->GlobalRotation = node->LocalRotation + root->Item->GlobalRotation;
			node->GlobalScale = node->LocalScale * root->Item->GlobalScale;
			parse(root->Childs[i]);
		}

	}
};


class SystemTransform2d
{
public:
	typedef core::FixedPool<ComponentTransform2d, GlobalConstants::ObjectsLimit> Pool;

	SystemTransform2d()
	{
	}

	float r;
	inline float createRand()
	{
		r += 0.1f;
		return r;
	}

	ComponentTransform2d * createRoot()
	{
		for (int i = 0; i < 128; i++)
			randomSwap[i] = createRand();
		randomIndex = 0;
		ComponentTransform2d *root = Pool::create();
		root->FirtsChildId = 0;
		root->NextNodeId = 0;
		root->Parent = 0;
		r = 0;

		return root;
	}

	float randomSwap[128];
	int randomIndex;

	inline float getRand()
	{
		++randomIndex;
		return randomSwap[randomIndex % 128];
	}

	inline void randomize(ComponentTransform2d *node)
	{
		node->LocalX = node->GlobalX = getRand();
		node->LocalX = node->GlobalY = getRand();
		node->LocalX = node->GlobalZ = getRand();

		node->LocalRotation = node->GlobalRotation = getRand();
		node->LocalScale = node->GlobalScale = getRand();
	}
		
	void create(ComponentTransform2d *root, int depth)
	{
		randomize(root);
		if (depth == 0) return;

		ComponentTransform2d *child = Pool::create();

		//alloc();

		child->Parent = root;
		child->NextNodeId = 0;
		child->FirtsChildId = 0;

		root->FirtsChildId = child;
		create(child, depth - 1);

		for (int i = 1; i < SIZE; i++)
		{
			ComponentTransform2d *nextChild = Pool::create();

			//alloc();

			nextChild->NextNodeId = 0;
			nextChild->FirtsChildId = 0;
			nextChild->Parent = root;

			//nextChild->NextNodeId = child;
			//root->FirtsChildId = nextChild;

			child->NextNodeId = nextChild;
			child = nextChild;
			create(nextChild, depth - 1);

		}
		/*
		child = root->FirtsChildId;
		while (child)
		{
		///alloc();
		create(child, depth - 1);
		child = child->NextNodeId;
		}
		*/

	}

	void notify(int id)
	{
	}

	void update()
	{
		// recalc global coords in transform tree
		//update aabb
	}

	inline void transform(ComponentTransform2d *node, ComponentTransform2d *parent)
	{
		node->GlobalX = node->LocalX + parent->GlobalX;
		node->GlobalY = node->LocalY + parent->GlobalY;
		node->GlobalZ = node->LocalZ + parent->GlobalZ;

		node->GlobalRotation = node->LocalRotation + parent->GlobalRotation;
		node->GlobalScale = node->LocalScale * parent->GlobalScale;
	}

	void parseLinear()
	{
		int count = SystemTransform2d::Pool::getCount();
		ComponentTransform2d * node;
		for (int i = 1; i < count; i++)
		{
			node = SystemTransform2d::Pool::getByIndex(i);
			node->GlobalX = node->LocalX + node->Parent->GlobalX;
			node->GlobalY = node->LocalY + node->Parent->GlobalY;
			node->GlobalZ = node->LocalZ + node->Parent->GlobalZ;

			node->GlobalRotation = node->LocalRotation + node->Parent->GlobalRotation;
			node->GlobalScale = node->LocalScale * node->Parent->GlobalScale;
			//transform(node, node->Parent);
		}
	}

	static int iterations;

	void parse(ComponentTransform2d *root)
	{
		//iterations++;
		// do recalc transform;
		ComponentTransform2d * node = root->FirtsChildId;
		while (node)
		{
			//transform(node, root);
			
			node->GlobalX = node->LocalX + root->GlobalX;
			node->GlobalY = node->LocalY + root->GlobalY;
			node->GlobalZ = node->LocalZ + root->GlobalZ;

			node->GlobalRotation = node->LocalRotation + root->GlobalRotation;
			node->GlobalScale = node->LocalScale * root->GlobalScale;
			
			if (node->FirtsChildId >= 0)
				parse(node);
			// do transform;
			if (node->NextNodeId >= 0)
			{
				node = node->NextNodeId;
			}
			else
			{
				node = 0;
			}
		}
	}

private:

	int _mask;
};

int SystemTransform2d::iterations(0);
int SystemTransform2dLink::iterations(0);

int doTest()
{
	__android_log_print(ANDROID_LOG_ERROR, "TRACKERS", "%s", "creation"); // dt
	SystemTransform2dNode::Pool::initialize();
	SystemTransform2dNode::ExPool::initialize();

	long start = clock();
	long finish = clock();
	long dt = finish - start;
	
	SystemTransform2d sys;
	SystemTransform2d::iterations = 0;
	ComponentTransform2d *root = sys.createRoot();
	sys.create(root, SIZE1);
	__android_log_print(ANDROID_LOG_ERROR, "TRACKERS", "%s", "creation done"); // dt
	int count = SystemTransform2d::Pool::getCount();


	start = getTimeNsec();
	for (int i = 0; i < 10; i++)
	{
	sys.parse(root);
	}
	finish = getTimeNsec();
	dt = finish - start;

	const char *intStr = to_string(dt).c_str();
	__android_log_print(ANDROID_LOG_ERROR, "TRACKERS", "%s", intStr); // dt
	const char *countStr = to_string(SystemTransform2d::iterations).c_str();
	__android_log_print(ANDROID_LOG_ERROR, "TRACKERS", "%s", countStr); // dt


	start = getTimeNsec();
	for (int i = 0; i < 10; i++)
	{
		sys.parseLinear();
	}
	finish = getTimeNsec();
	dt = finish - start;

	intStr = to_string(dt).c_str();
	__android_log_print(ANDROID_LOG_ERROR, "TRACKERS", "%s", intStr); // dt
	countStr = to_string(count).c_str();
	__android_log_print(ANDROID_LOG_ERROR, "TRACKERS", "%s", countStr); // dt

	
	__android_log_print(ANDROID_LOG_ERROR, "TRACKERS", "%s", "creation"); // dt
	
	SystemTransform2dLink sysLink;
	SystemTransform2dNode *rootLink = sysLink.createRoot();
	SystemTransform2dLink::iterations = 0;
	sysLink.create(rootLink, SIZE1);
	__android_log_print(ANDROID_LOG_ERROR, "TRACKERS", "%s", "creation done"); // dt
	
	SystemTransform2dLink::iterations = 0;


	start = getTimeNsec();
	for (int i = 0; i < 10; i++)
	{
		sysLink.parse(rootLink);
	}

	finish = getTimeNsec();
	dt = finish - start;

	intStr = to_string(dt).c_str();
	__android_log_print(ANDROID_LOG_ERROR, "TRACKERS", "%s", intStr); // dt
	countStr = to_string(SystemTransform2dLink::iterations).c_str();
	__android_log_print(ANDROID_LOG_ERROR, "TRACKERS", "%s", countStr); // dt

	return 0;
}



static int engine_init_display(struct engine* engine) {

    // initialize OpenGL ES and EGL

    /*
     * Here specify the attributes of the desired configuration.
     * Below, we select an EGLConfig with at least 8 bits per color
     * component compatible with on-screen windows
     */
    const EGLint attribs[] = {
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT, 
        EGL_BLUE_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_RED_SIZE, 8,
        EGL_NONE
    };
    EGLint w, h, dummy, format;
    EGLint numConfigs;
    EGLConfig config;
    EGLSurface surface;
    EGLContext context;

    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    eglInitialize(display, 0, 0);

    /* Here, the application chooses the configuration it desires. In this
     * sample, we have a very simplified selection process, where we pick
     * the first EGLConfig that matches our criteria */
    eglChooseConfig(display, attribs, &config, 1, &numConfigs);

    /* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
     * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
     * As soon as we picked a EGLConfig, we can safely reconfigure the
     * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

    ANativeWindow_setBuffersGeometry(engine->app->window, 0, 0, format);

    surface = eglCreateWindowSurface(display, config, engine->app->window, NULL);

	EGLint attrib_list[3] = {
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};
	context = eglCreateContext(display, config, NULL, attrib_list);
	
    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
        LOGW("Unable to eglMakeCurrent");
        return -1;
    }

    eglQuerySurface(display, surface, EGL_WIDTH, &w);
    eglQuerySurface(display, surface, EGL_HEIGHT, &h);

    engine->display = display;
    engine->context = context;
    engine->surface = surface;
    engine->width = w;
    engine->height = h;
    engine->state.angle = 0;
	engine->animating = 1;

    // Initialize GL state.

	int error = 0;
 //   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
	//error = glGetError();
	glEnable(GL_CULL_FACE);
	error = glGetError();
	glShadeModel(GL_SMOOTH);
	error = glGetError();
	glDisable(GL_DEPTH_TEST);
	error = glGetError();
	
	_factory = llge::createFactory();
	_renderSystem = _factory->createRenderSystem();
	_renderSystem->create();

	doTest();

	return 0;
}

/**
 * Just the current frame in the display.
 */
static void engine_draw_frame(struct engine* engine) {


    if (engine->display == NULL) {
        // No display.
        return;
    }

    // Just fill the screen with a color.
	/*
    glClearColor(((float)engine->state.x)/engine->width, engine->state.angle,
            ((float)engine->state.y)/engine->height, 1);
    glClear(GL_COLOR_BUFFER_BIT);
	*/

	_renderSystem->viewport(engine->width, engine->height);
	_renderSystem->render();

    eglSwapBuffers(engine->display, engine->surface);
}

/**
 * Tear down the EGL context currently associated with the display.
 */
static void engine_term_display(struct engine* engine) {
    if (engine->display != EGL_NO_DISPLAY) {
        eglMakeCurrent(engine->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (engine->context != EGL_NO_CONTEXT) {
            eglDestroyContext(engine->display, engine->context);
        }
        if (engine->surface != EGL_NO_SURFACE) {
            eglDestroySurface(engine->display, engine->surface);
        }
        eglTerminate(engine->display);
    }
    engine->animating = 0;
    engine->display = EGL_NO_DISPLAY;
    engine->context = EGL_NO_CONTEXT;
    engine->surface = EGL_NO_SURFACE;
}

/**
 * Process the next input event.
 */
static int32_t engine_handle_input(struct android_app* app, AInputEvent* event) {
    struct engine* engine = (struct engine*)app->userData;
    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
        engine->animating = 1;
        engine->state.x = AMotionEvent_getX(event, 0);
        engine->state.y = AMotionEvent_getY(event, 0);
        return 1;
    }
    return 0;
}

/**
 * Process the next main command.
 */
static void engine_handle_cmd(struct android_app* app, int32_t cmd) {
    struct engine* engine = (struct engine*)app->userData;
    switch (cmd) {
        case APP_CMD_SAVE_STATE:
            // The system has asked us to save our current state.  Do so.
            engine->app->savedState = malloc(sizeof(struct saved_state));
            *((struct saved_state*)engine->app->savedState) = engine->state;
            engine->app->savedStateSize = sizeof(struct saved_state);
            break;
        case APP_CMD_INIT_WINDOW:
            // The window is being shown, get it ready.
            if (engine->app->window != NULL) {
                engine_init_display(engine);
                engine_draw_frame(engine);
            }
            break;
        case APP_CMD_TERM_WINDOW:
            // The window is being hidden or closed, clean it up.
            engine_term_display(engine);
            break;
        case APP_CMD_GAINED_FOCUS:
            // When our app gains focus, we start monitoring the accelerometer.
            if (engine->accelerometerSensor != NULL) {
                ASensorEventQueue_enableSensor(engine->sensorEventQueue,
                        engine->accelerometerSensor);
                // We'd like to get 60 events per second (in us).
                ASensorEventQueue_setEventRate(engine->sensorEventQueue,
                        engine->accelerometerSensor, (1000L/60)*1000);
            }
            break;
        case APP_CMD_LOST_FOCUS:
            // When our app loses focus, we stop monitoring the accelerometer.
            // This is to avoid consuming battery while not being used.
            if (engine->accelerometerSensor != NULL) {
                ASensorEventQueue_disableSensor(engine->sensorEventQueue,
                        engine->accelerometerSensor);
            }
            // Also stop animating.
            engine->animating = 0;
            engine_draw_frame(engine);
            break;
    }
}

/**
 * This is the main entry point of a native application that is using
 * android_native_app_glue.  It runs in its own thread, with its own
 * event loop for receiving input events and doing other things.
 */
void android_main(struct android_app* state) {
    struct engine engine;

    // Make sure glue isn't stripped.
    app_dummy();

    memset(&engine, 0, sizeof(engine));
    state->userData = &engine;
    state->onAppCmd = engine_handle_cmd;
    state->onInputEvent = engine_handle_input;
    engine.app = state;

    // Prepare to monitor accelerometer
    engine.sensorManager = ASensorManager_getInstance();
    engine.accelerometerSensor = ASensorManager_getDefaultSensor(engine.sensorManager,
            ASENSOR_TYPE_ACCELEROMETER);
    engine.sensorEventQueue = ASensorManager_createEventQueue(engine.sensorManager,
            state->looper, LOOPER_ID_USER, NULL, NULL);

    if (state->savedState != NULL) {
        // We are starting with a previous saved state; restore from it.
        engine.state = *(struct saved_state*)state->savedState;
    }

    // loop waiting for stuff to do.

    while (1) {
        // Read all pending events.
        int ident;
        int events;
        struct android_poll_source* source;

        // If not animating, we will block forever waiting for events.
        // If animating, we loop until all events are read, then continue
        // to draw the next frame of animation.
        while ((ident=ALooper_pollAll(engine.animating ? 0 : -1, NULL, &events,
                (void**)&source)) >= 0) {

            // Process this event.
            if (source != NULL) {
                source->process(state, source);
            }

            // If a sensor has data, process it now.
            if (ident == LOOPER_ID_USER) {
                if (engine.accelerometerSensor != NULL) {
                    ASensorEvent event;
                    while (ASensorEventQueue_getEvents(engine.sensorEventQueue,
                            &event, 1) > 0) {
                        LOGI("accelerometer: x=%f y=%f z=%f",
                                event.acceleration.x, event.acceleration.y,
                                event.acceleration.z);
                    }
                }
            }

            // Check if we are exiting.
            if (state->destroyRequested != 0) {
                engine_term_display(&engine);
                return;
            }
        }

        if (engine.animating) {
            // Done with events; draw next animation frame.
            engine.state.angle += .01f;
            if (engine.state.angle > 1) {
                engine.state.angle = 0;
            }

            // Drawing is throttled to the screen update rate, so there
            // is no need to do timing here.
            engine_draw_frame(&engine);
        }
    }
}
//END_INCLUDE(all)
